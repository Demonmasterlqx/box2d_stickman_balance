#include "render.hpp"
#include <cmath>

Renderer::Renderer(b2WorldDef worldDef, float timeStep, int subStepCount)
    : timeStep(timeStep), subStepCount(subStepCount)
{
    worldId = b2CreateWorld(&worldDef);
}

void Renderer::addBody(const Object& body) {
    // 完全复制 Object 并创建物理体
    Object obj = body;
    obj.bodyId = b2CreateBody(worldId, &obj.bodyDef);
    b2CreatePolygonShape(obj.bodyId, &obj.shapeDef, &obj.polygon);
    ObjectList.push_back(obj);
}

void Renderer::updateFrame(GLFWwindow* window) {
    // 更新物理世界
    b2World_Step(worldId, timeStep, subStepCount);

    // 动态获取窗口大小
    int display_w = 1280, display_h = 720;
    if (window) glfwGetFramebufferSize(window, &display_w, &display_h);
    float scale = display_h / 18.0f; // 让物理世界18米高正好填满窗口
    ImVec2 screenOffset(display_w / 2.0f, display_h / 2.0f);

    for (const auto& obj : ObjectList) {
        if (!b2Body_IsValid(obj.bodyId)) continue;
        b2Vec2 pos = b2Body_GetPosition(obj.bodyId);
        float angle = b2Rot_GetAngle(b2Body_GetRotation(obj.bodyId));
        int count = obj.polygon.count;
        std::vector<ImVec2> points;
        points.reserve(count);
        for (int i = 0; i < count; ++i) {
            float x = obj.polygon.vertices[i].x;
            float y = obj.polygon.vertices[i].y;
            float rx = x * cosf(angle) - y * sinf(angle);
            float ry = x * sinf(angle) + y * cosf(angle);
            float sx = screenOffset.x + (pos.x + rx) * scale;
            float sy = screenOffset.y - (pos.y + ry) * scale;
            points.emplace_back(sx, sy);
        }
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        if (!points.empty())
            draw_list->AddPolyline(points.data(), points.size(), IM_COL32(255,255,0,255), true, 2.0f);
    }
}

Renderer::~Renderer() {
    // 清理物理世界
    if (B2_IS_NON_NULL(worldId)) {
        b2DestroyWorld(worldId);
        worldId = b2_nullWorldId;
    }
    ObjectList.clear();
}
