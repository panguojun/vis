/**
                拓扑关系可视化
            这里根据外部数据显示拓扑图
*/
#define Vec2    ImVec2 
inline ImVec2 VEC2(const vec2& v) { return ImVec2(v.x, v.y); }

#include "AI/grid_planning.hpp"
#include "AI/path_astar.hpp"

namespace topo_view
{
    using topo_seg_t = pair<char, char>;
    vector<topo_seg_t> topo_list = {
        {1, 2},{1, 3},{1, 4},{2, 2},{2, 3},{3, 4},{1, 5},{4, 5}
    };

    grid_planning gridspace;
    ASTAR path;

    // 函数用来绘制箭头
    void draw_arrow(ImDrawList* drawList, vec2 start, vec2 end, ImU32 color, float thickness) {
        // 绘制箭头的主线
        drawList->AddLine(VEC2(start), VEC2(end), color, thickness);

        // 计算箭头的方向
        vec2 v = end - start;
        crd2 C;C.rot2dir(v.normalized());

        // 计算箭头的两侧
        vec2 arrowPoint1 = end - C.ux * 15.0f + C.uy * 5.0f;
        vec2 arrowPoint2 = end - C.ux * 15.0f - C.uy * 5.0f;

        // 绘制箭头的两侧
        drawList->AddTriangleFilled(VEC2(arrowPoint1), VEC2(end), VEC2(arrowPoint2), color);
    }

    // 展示拓扑关系可视化
    void draw()
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        for (int i = 0; i < topo_list.size(); i++)
        {
            point2 new_pos1 = gridspace.gen_node(topo_list[i].first);
            point2 new_pos2 = gridspace.gen_node(topo_list[i].second);

            path.init();
            path.findpath({ new_pos1.x, new_pos1.y }, { new_pos2.x, new_pos2.y });

            // 定义两个方块的位置和大小
            vec2 box1Pos(new_pos1.x * 200, new_pos1.y * 100 + 100);
            vec2 box2Pos(new_pos2.x * 200, new_pos2.y * 100 + 100);
            vec2 boxSize(100, 50);

            // 定义颜色
            ImU32 boxColor = IM_COL32(255, 255, 255, 255);  // 白色
            ImU32 arrowColor = IM_COL32(255, 0, 0, 255);    // 红色
            float arrowThickness = 2.0f;

            // 绘制方块
            drawList->AddRectFilled(VEC2(box1Pos), VEC2(box1Pos + boxSize), boxColor);
            drawList->AddRect(VEC2(box1Pos), VEC2(box1Pos + boxSize), IM_COL32(0, 0, 0, 255)); // 方块边框

            drawList->AddRectFilled(VEC2(box2Pos), VEC2(box2Pos + boxSize), boxColor);
            drawList->AddRect(VEC2(box2Pos), VEC2(box2Pos + boxSize), IM_COL32(0, 0, 0, 255)); // 方块边框

            // 计算箭头的起始和结束位置
        
            vec2 arrowStart(box1Pos.x + boxSize.x, box1Pos.y + boxSize.y / 2);
            vec2 arrowEnd(box2Pos.x, box2Pos.y + boxSize.y / 2);

            // 绘制箭头
            draw_arrow(drawList, arrowStart, arrowEnd, arrowColor, arrowThickness);
        }
    }
}