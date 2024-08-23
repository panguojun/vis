/**
*           【 场景模型的矩阵视图 】
* 
*/
#include    "xmat.hpp"
#include    "../Xmatrix/core/xastar.hpp"
#include    "../Xmatrix/xprimitive.hpp"
#include    "../Xmatrix/xtriangle.hpp"
#include    "../Xmatrix/xsubmesh.hpp"

namespace ScePHG {
#include    "MFRU/detection_head.hpp"
}

#define GRID_NUM     5
#define GRID_SIZE    100
#define SM_SCALE     25

namespace fs = std::filesystem;
extern EXPORT_API submesh read_submesh_bin(const std::string& filename);
// ------------------------------------
// 模型空间定位
// ------------------------------------
crd3 get_tri_crdEX(const submesh& sm, int tri, crcd3 C)
{
    crd3 c = crd3::ZERO;
    if (sm.empty())
        return c;

    crvec p1 = sm.get_tri_ver(tri, 0).p;
    crvec p2 = sm.get_tri_ver(tri, 1).p;
    crvec p3 = sm.get_tri_ver(tri, 2).p;

    c.ux = (p2 - p1);
    c.uz = c.ux.cross(p3 - p1).normalized();
    c.uy = c.ux.cross(c.uz).normalized();
    c.uy = c.uy * (p3 - p1).dot(c.uy);

    for (int i = 0; i < sm.vertices.size(); i++)
    {
        const vertex& v = sm.vertices[i];
        vec3 vz = v.p - p1;
        real z = vz.dot(c.uz);
        if (::abs(z) > c_tor_dis_level0)
        {
            c.uz *= z;
            break;
        }
    }
    c.norm();

    return c;
}
// *******************************************
// grid_canvas_scene
// *******************************************
namespace grid_canvas_scene
{
    Xmatrix<Xsubmesh> m1(1, 1);
    vector<std::shared_ptr<submesh>> find_submesh_in_folder(const std::string& folderPath)
    {
        vector<std::shared_ptr<submesh>> ret;

        for (const auto& entry : fs::directory_iterator(folderPath))
        {
            if (entry.path().extension() == ".sm")
            {
                ret.emplace_back(
                    std::make_shared<submesh>(read_submesh_bin(entry.path().string())));
            }
        }

        PRINTV(ret.size());

        return ret;
    }

    // 设置矩阵
    void setup_mat(const crd2& C, const std::string& folderPath)
    {
        auto one = Xsm_ONE;
        int sz = GRID_NUM;
        m1 = Xmatrix<Xsubmesh>(sz, sz);
        for (int i = 0; i < sz; i++)
        {
            for (int j = 0; j < sz; j++)
            {
                m1.data[i][j] = one;
            }
        }

        int cnt = 0;
        auto list = find_submesh_in_folder(folderPath);
        for (auto& sm_ptr : list)
        {
            int i = cnt / sz;
            int j = cnt % sz;
            if (i >= sz)
                break;

            m1.data[i][j].sm = move(*sm_ptr);

            crd3 c_md = get_tri_crdEX(m1.data[i][j].sm, 0, vec3::ZERO);
            c_md.norm(false);
            m1.data[i][j].sm /= c_md;

            cnt++;
        }
    }

    // 绘制矩阵
    void draw_mat(const Xmatrix<Xsubmesh>& x)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        int size = GRID_SIZE;

        int row = 0;
        for (auto& e : x.data)
        {
            int col = 0;
            for (auto& v : e)
            {
                const vec2& o = vec2(150 + col * size, 150 + row * size);

                ImVec2 topLeft = ImVec2(o.x - size / 2, o.y - size / 2);
                ImVec2 topRight = ImVec2(o.x + size / 2, o.y - size / 2);
                ImVec2 bottomLeft = ImVec2(o.x - size / 2, o.y + size / 2);
                ImVec2 bottomRight = ImVec2(o.x + size / 2, o.y + size / 2);

                drawList->AddLine(topLeft, topRight, IM_COL32(255, 155, 0, 55), 1.0f);
                drawList->AddLine(topRight, bottomRight, IM_COL32(255, 155, 0, 55), 1.0f);
                drawList->AddLine(bottomRight, bottomLeft, IM_COL32(255, 155, 0, 55), 1.0f);
                drawList->AddLine(bottomLeft, topLeft, IM_COL32(255, 255, 0, 55), 1.0f);

                static vec3 anchor = v.sm.get_tri_ver(0, 0).p;
                for (int i = 0; i < v.sm.tris.size(); i++)
                {
                    triangle3 tri = { v.sm.get_tri_ver(i, 0).p, v.sm.get_tri_ver(i, 1).p, v.sm.get_tri_ver(i, 2).p };

                    vec2 A = o + tri.a.xz() * SM_SCALE;
                    vec2 B = o + tri.b.xz() * SM_SCALE;
                    vec2 C = o + tri.c.xz() * SM_SCALE;

                    drawList->AddTriangle(ImVec2(A.x, A.y), ImVec2(B.x, B.y), ImVec2(C.x, C.y), IM_COL32(255, 255, 255, 255));
                }
                col++;
            }
            row++;
        }
    }
    // *******************************************
    // 绘制函数
    // *******************************************
    void display()
    {
        static bool binst = false;
        if (!binst)
        {
            setup_mat(vec2(0, 0), "C:/Users/18858/Desktop/GDB/scene/1");
            binst = true;
        }
        draw_mat(m1);
    }
}