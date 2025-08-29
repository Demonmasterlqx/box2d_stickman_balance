#include "renderer/render.hpp"
#include <thread>
#include <chrono>
#include "box2d/box2d.h"
#include "box2d/types.h"

std::vector<b2JointId> generate_legs(Renderer& renderer, const b2Vec2& worldPivot){

    std::vector<b2BodyId> uplegIds;
    std::vector<b2JointId> jointIds;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    shapeDef.filter.categoryBits = CATEGORY_PLAYER1;
    shapeDef.filter.maskBits = ~CATEGORY_PLAYER1;

    for(int i=0;i<2;i++){

        b2BodyDef head_leg = b2DefaultBodyDef();
        head_leg.type = b2_dynamicBody;
        head_leg.position = {worldPivot.x, worldPivot.y + 0.35f};
        b2Polygon head_leg_dynamicBox = b2MakeBox(0.05f, 0.35f);
        b2BodyId head_leg_id = renderer.addBody(Object(head_leg, head_leg_dynamicBox, shapeDef));

        uplegIds.push_back(head_leg_id);

        b2BodyDef down_leg = b2DefaultBodyDef();
        down_leg.type = b2_dynamicBody;
        down_leg.position = {worldPivot.x, worldPivot.y + 0.7f + 0.5f};
        b2Polygon down_leg_dynamicBox = b2MakeBox(0.05f, 0.5f);
        b2BodyId down_leg_id = renderer.addBody(Object(down_leg, down_leg_dynamicBox, shapeDef));

        b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();

        jointDef.base.bodyIdA = head_leg_id;
        jointDef.base.bodyIdB = down_leg_id;
        jointDef.lowerAngle = -B2_PI / 2;
        jointDef.upperAngle = B2_PI / 2;
        jointDef.enableLimit = true;

        jointDef.base.localFrameA.p = (b2Vec2){0, -0.35f};
        jointDef.base.localFrameB.p = (b2Vec2){0, 0.5f};

        jointDef.enableMotor = true;
        jointDef.maxMotorTorque = 10.0f;
        jointDef.base.collideConnected = false;

        b2JointId id=renderer.addRevoluteJoint(jointDef);

        b2RevoluteJoint_SetMaxMotorTorque(id,2.0f);

        jointIds.push_back(id);
    }

    b2BodyDef empty_node = b2DefaultBodyDef();
    empty_node.type = b2_dynamicBody;
    empty_node.position = {worldPivot.x, worldPivot.y};
    b2Polygon empty_node_shape = b2MakeBox(0.05f, 0.05f);
    b2BodyId empty_node_id = renderer.addBody(Object(empty_node, empty_node_shape, shapeDef));

    for(int i=0;i<2;i++){
        b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();

        jointDef.base.bodyIdA = empty_node_id;
        jointDef.base.bodyIdB = uplegIds[i];

        jointDef.base.localFrameA.p = (b2Vec2){0, 0.0f};
        jointDef.base.localFrameB.p = (b2Vec2){0, 0.35f};

        jointDef.enableMotor = true;
        jointDef.maxMotorTorque = 10.0f;
        jointDef.base.collideConnected = false;

        b2JointId id=renderer.addRevoluteJoint(jointDef);

        b2RevoluteJoint_SetMaxMotorTorque(id,2.0f);

        jointIds.push_back(id);
    }

    return jointIds;

}

int main() {
    // 1. 初始化GLFW库
    if (!glfwInit()) {
        return -1;
    }
    // 2. 创建窗口和OpenGL上下文
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Box2D + ImGui Demo", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
#ifdef IMGUI_IMPL_OPENGL_LOADER_GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize OpenGL loader!\n");
        return -1;
    }
#endif
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // 物理世界和渲染器初始化
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, -10.0f};
    worldDef.workerCount = 4;
    Renderer renderer(worldDef);

    // 添加地面
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = {0.0f, -5.0f};
    b2Polygon groundBox = b2MakeBox(50.0f, 0.2f);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    groundShapeDef.filter.categoryBits = CATEGORY_SURROUNDING;
    renderer.addBody(Object(groundBodyDef, groundBox, groundShapeDef));

    // 添加一个动态方块
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {0.0f, 0.0f};
    b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.filter.categoryBits = CATEGORY_SURROUNDING;
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    renderer.addBody(Object(bodyDef, dynamicBox, shapeDef));

    std::vector<b2JointId> myJointId=generate_legs(renderer, (b2Vec2){1.2,1});

    // 主渲染循环
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 可选：显示ImGui Demo窗口
        // ImGui::ShowDemoWindow();

        // 更新物理世界并渲染（自适应窗口大小）
        // for(int i=0;i<myJointId.size();i++){
            b2RevoluteJoint_SetMotorSpeed(myJointId[0], 0.11f);
        // }
        renderer.updateFrame(window);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        // std::this_thread::sleep_for(std::chrono::milliseconds(10000000)); // 控制帧率
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
