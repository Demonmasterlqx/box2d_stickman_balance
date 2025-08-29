
#include "imgui/imgui.h"                 // ImGui 主头文件
#include "imgui/imgui_impl_glfw.h"       // ImGui GLFW 平台绑定
#include "imgui/imgui_impl_opengl3.h"    // ImGui OpenGL3 渲染绑定
#include <GLFW/glfw3.h>                  // GLFW 头文件
#include <thread>                        // C++11 线程库
#ifdef IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>                   // OpenGL loader（如使用 glad）
#endif

int main(){
    // 1. 初始化GLFW库
    if (!glfwInit()) {
        // 初始化失败直接退出
        return -1;
    }

    // 2. 创建窗口和OpenGL上下文
    // 参数：宽1280，高720，标题，窗口模式
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui + GLFW + OpenGL3", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    // 设为当前OpenGL上下文
    glfwMakeContextCurrent(window);
    // 开启垂直同步，防止画面撕裂
    glfwSwapInterval(1);

#ifdef IMGUI_IMPL_OPENGL_LOADER_GLAD
    // 3. 初始化OpenGL loader（如glad）
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize OpenGL loader!\n");
        return -1;
    }
#endif

    // 4. 初始化ImGui上下文
    IMGUI_CHECKVERSION(); // 检查ImGui版本
    ImGui::CreateContext(); // 创建ImGui上下文
    ImGuiIO& io = ImGui::GetIO(); // 获取IO对象
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘导航
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // 启用手柄导航

    ImGui::StyleColorsDark(); // 设置深色主题

    // 5. 初始化ImGui平台/渲染绑定
    ImGui_ImplGlfw_InitForOpenGL(window, true); // 绑定GLFW
    ImGui_ImplOpenGL3_Init();                   // 绑定OpenGL3

    // 6. 主渲染循环
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // 处理事件（如键盘、鼠标等）

        // 各平台/渲染器新帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 显示ImGui自带的Demo窗口
        ImGui::ShowDemoWindow();

        ImGui::Render(); // 生成渲染数据
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h); // 获取帧缓冲区大小
        glViewport(0, 0, display_w, display_h);                 // 设置视口
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);                   // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT);                           // 清屏
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // 用ImGui渲染

        glfwSwapBuffers(window); // 交换前后缓冲，显示内容
    }

    // 7. 资源释放和清理
    ImGui_ImplOpenGL3_Shutdown(); // 关闭ImGui OpenGL3绑定
    ImGui_ImplGlfw_Shutdown();    // 关闭ImGui GLFW绑定
    ImGui::DestroyContext();      // 销毁ImGui上下文
    glfwDestroyWindow(window);    // 销毁窗口
    glfwTerminate();              // 关闭GLFW
    return 0;
}