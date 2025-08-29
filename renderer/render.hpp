#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "box2d/box2d.h"

#include "imgui.h"                 // ImGui 主头文件
#include "imgui_impl_glfw.h"       // ImGui GLFW 平台绑定
#include "imgui_impl_opengl3.h"    // ImGui OpenGL3 渲染绑定
#include <GLFW/glfw3.h>                  // GLFW 头文件
#include <thread>                        // C++11 线程库
#ifdef IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>                   // OpenGL loader（如使用 glad）
#endif


enum CollisionCategory
{
    CATEGORY_PLAYER1 = 0x0001, // 0000000000000001
    CATEGORY_PLAYER2  = 0x0002, // 0000000000000010
    CATEGORY_PLAYER3   = 0x0004, // 0000000000000100
    CATEGORY_PLAYER4 = 0x0008, // 0000000000001000
    CATEGORY_SURROUNDING = 0x0010, // 0000000000010000
};

/**
 * @brief 这个结构体用来表示一个可渲染的物体
 * 
 */
struct Object {
    b2BodyDef bodyDef;
    b2Polygon polygon;
    b2ShapeDef shapeDef;
    b2BodyId bodyId = b2_nullBodyId;

    Object(const b2BodyDef& bodyDef, const b2Polygon& polygon, const b2ShapeDef& shapeDef)
        : bodyDef(bodyDef), polygon(polygon), shapeDef(shapeDef) {}
};

class Renderer {
    private:
        // 当前世界的 ID
        b2WorldId worldId = b2_nullWorldId;
        // 当前时间步长
        float timeStep;
        // 当前子步数
        int subStepCount;
        // 物体列表
        std::vector<Object> ObjectList;
    public:

    /**
     * @brief Construct a new Renderer object
     * 
     * 函数将会根据提供的世界定义 得到一个新的 b2WorldId
     * 
     * @param worldDef 
     * @param timeStep 
     * @param subStepCount 
     */
    Renderer(b2WorldDef worldDef, float timeStep = 1.0f / 60.0f, int subStepCount = 4);

    /**
     * @brief 添加一个新的物体到渲染器中,并且完全复制这个物体的信息
     * 
     * @param body 物体
     */
    b2BodyId addBody(const Object & body);

    /**
     * @brief 添加一个新的铰接关节到渲染器中
     * 
     * @param jointDef 
     * @return b2JointId 
     */
    b2JointId addRevoluteJoint(const b2RevoluteJointDef & jointDef);

    /**
     * @brief 函数将会更新world并将其渲染到屏幕上
     * 
     */
    void updateFrame(GLFWwindow* window);

    /**
     * @brief Destroy the Renderer object 清理上下文资源
     * 
     */
    ~Renderer();
};