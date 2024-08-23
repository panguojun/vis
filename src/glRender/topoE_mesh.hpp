/**				常用的几何拓扑库
*/
namespace TopoLib
{
    inline bool VERLIST_ADD(int64 tag, VERLIST& e, crvec p) { if (e.empty() || e.back().p != p) { e.emplace_back(p, -1, tag); return true; } return false; }

    inline void roundedge(VERLIST& e, crcd3 c, real r, int sig, real ang0){ roundedge(e, c.o, c.ux, c.uz, r, sig, ang0);}

    inline void face(vertex& p1, vertex& p2, vertex& p3, bool autonum = 1)
    {
        if (autonum) {
            vec3 n = binvnorm ? -NORMV(p1, p2, p3) : NORMV(p1, p2, p3);
            n.norm();
            p1.n = n;
            p2.n = n;
            p3.n = n;
        }
        ::triang(p1, p2, p3);
    }
    inline void face0(vertex&& p1, vertex&& p2, vertex&& p3, bool autonum = 1)
    {
        if (autonum) {
            vec3 n = binvnorm ? -NORMV(p1, p2, p3) : NORMV(p1, p2, p3);
            n.norm();
            p1.n = n;
            p2.n = n;
            p3.n = n;
        }
        ::triang0(p1, p2, p3);
    }
    inline void plane(vertex& p1, vertex& p2, vertex& p3, vertex& p4, bool autonum = 1)
    {
        if (autonum) {
            vec3 n = binvnorm ? -NORMV(p1, p2, p3) : NORMV(p1, p2, p3);
            n.norm();
            p1.n = n;
            p2.n = n;
            p3.n = n;
            p4.n = n;
        }
        ::plane(p1, p2, p3, p4);
    }
    // 带孔洞的拓扑边
    inline void topo_edge_holeL(VERLIST& e, topoE& te, vec3 p_set[])
    {
        //color = RED;
        //vec3 p0;
        te.walkL([&](topoE* to, int i, real t) {
            vec3 p;
            if (!to->parent || !to->parent->bmutex) {
                p = blend(p_set[to->e.start], p_set[to->e.end], t);
            }
            else {// 自动生成圆形孔洞
                coord3 c1(p_set[to->e.start]), c2(p_set[to->e.end]);
                p = blender::slerp(c1, c2, (c1 + c2) / 2.0f, t).o;
               // pt3d(p);
            }
            if (VERLIST_ADD((int64)to, e, p)) {
                /* PRINT("(" << e.size() << ") " << to->name);
                 pt3d(p, 0.01 * e.size());
                 if (p0 != vec3::ZERO)
                     ptr(p0, p, 0.01);*/
               // p0 = p;
            }
            });
    }
    inline void topo_edge_holeR(VERLIST& e, topoE& te, vec3 p_set[])
    {
        //color = BLUE;
       // vec3 p0;
        te.walkR([&](topoE* to, int i, real t) {
            vec3 p;
            if (!to->parent || !to->parent->bmutex) {
                p = blend(p_set[to->e.start], p_set[to->e.end], t);
            }
            else {// 自动生成圆形孔洞
                coord3 c1(p_set[to->e.start]), c2(p_set[to->e.end]);
                p = blender::slerp_conjcopy(c1, c2, (c1 + c2) / 2.0f, t).o;
                //pt3d(p);
            }
            if (VERLIST_ADD((int64)to, e, p)) {
                /*  PRINT("(" << e.size() << ") " << to->name);
                  pt3d(p, 0.01 * e.size());
                  if (p0 != vec3::ZERO)
                      ptr(p0, p, 0.01);*/
                //p0 = p;
            }
            });
    }
}