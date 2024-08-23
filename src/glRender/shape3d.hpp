/**				绘制3D几何对象
*/
// -----------------------------------------------
// primitive 3d
// -----------------------------------------------
namespace shape3d
{
    coord3  cur_coord;

    // 绘制点：

    void drawPoint(float x, float y, float z) {
        crd3 c = cur_coord;
        c.o = vec3(x, y, z);
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(c.o.x, c.o.y, c.o.z);
        glEnd();
    }

    // 绘制线段：

    void drawLine(float startX, float startY, float startZ, float endX, float endY, float endZ, float width = 2) {
        glPushMatrix();
        glLineWidth(width);
        glBegin(GL_LINES);
        glVertex3f(startX, startY, startZ);
        glVertex3f(endX, endY, endZ);
        glEnd();
        glPopMatrix();
    }

    // 绘制多段线：

    void drawPolygon(std::vector<float> vertices) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < vertices.size(); i += 3) {
            glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
        }
        glEnd();
    }

    // 绘制圆

    void drawCircle(const crd3& c, real r) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 8; i ++) {
            float angle = (i / 8.0f) * PI * 2;
            vec3 p = c.o + c.ux * (r * cos(angle)) + c.uy * (r * sin(angle));
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
    }
    // 绘制圆柱：
    
    void drawCylinder(float radius, float height) {
        binvnorm = true;
        color = 0xFFFFFFFF;
        crd3 c;
        VERLIST e1;  TopoLib::roundedge(e1, c, radius, 8, 0);
        c += vec3::UY * height;
        VERLIST e2;  TopoLib::roundedge(e2, c, radius, 8, 0);

        for (auto& v : e1)
            v *= cur_coord;
        for (auto& v : e2)
            v *= cur_coord;

        Lineiter space = { 0, 1, 2 };
        space.walk([&e1, &e2](int i, real x) {
            face(e1, e2);
            });
        binvnorm = false;
    }

    // 绘制球体：

    void drawSphere(float radius, int slices=8, int stacks=8) {
        binvnorm = true;
        color = 0xFFFFFFFF;
        crd3 c;
        VERLIST vertices;
        for (int i = 0; i <= stacks; i++) {
            float v = i / (float)stacks;
            float phi = v * PI;
            for (int j = 0; j <= slices; j++) {
                float u = j / (float)slices;
                float theta = u * PI * 2;
                float x = cos(theta) * sin(phi);
                float y = cos(phi);
                float z = sin(theta) * sin(phi);
                vertices.push_back((vec3(x, y, z) * radius) * cur_coord);
            }
        }

        for (int i = 0; i < slices * stacks; i++) {
            int i1 = i;
            int i2 = i + 1;
            int i3 = i + slices + 1;
            int i4 = i + slices + 2;
           // TopoLib::plane(vertices[i1], vertices[i2], vertices[i4], vertices[i3]);
            TopoLib::face(vertices[i1], vertices[i2], vertices[i3]);
            TopoLib::face(vertices[i2], vertices[i4], vertices[i3]);
        }
        binvnorm = false;
    }

    // 球冠

    void drawSphereCrown0(float radius,
        float h, // 高度
        float k, // 曲率
        int slices = 18, int stacks = 18) {
       // binvnorm = true;
       // bcomvertex = true;
        color = 0xFFFFFFFF;
        crd3 c;
        VERLIST vertices;
        for (int i = stacks / 2; i <= stacks; i++) {
            float v = i / (float)stacks;
            float phi = PI - v * PI;
            for (int j = 0; j <= slices; j++) {
                float u = j / (float)slices;
                float theta = u * PI * 2;
                float x = cos(theta) * sin(phi);
                float y = cos(phi);
                float z = sin(theta) * sin(phi);
                vec3 point(x, y, z);
                float dist = point.len();
                if (dist > 1.0f) {
                    point.norm();
                    point *= radius;
                    point.y += h;
                    point.y += k * ((point.x * point.x + point.z * point.z) / (radius * radius));
                    vec3 norm = point / point.len();
                    vertices.emplace_back(point * cur_coord, norm);
                }
                else {
                    point *= radius;
                    point.y += h;
                    point.y += k * ((point.x * point.x + point.z * point.z) / (radius * radius));
                    vec3 norm = point / point.len();
                    vertices.emplace_back(point * cur_coord, norm);
                }
            }
        }

        for (int i = 0; i < slices * stacks / 2; i++) {
            int i1 = i;
            int i2 = i + 1;
            int i3 = i + slices + 1;
            int i4 = i + slices + 2;

            TopoLib::face(vertices[i1], vertices[i2], vertices[i3], false);
            TopoLib::face(vertices[i2], vertices[i4], vertices[i3], false);
        }
       // binvnorm = false;
        //bcomvertex = false;
    }

    // 半球

    void drawHemiSphere(float radius, int slices = 18, int stacks = 18) {
        bcomvertex = 1;
        real k = 0.2;
        VERLIST e0;
        roundedge(e0, vec3::ZERO, vec3::UX, vec3::UZ, radius, slices, 0);
        calcroundnorm(e0, vec3::ZERO);
        {
            VERLIST e;
            vec3 oo = vec3::ZERO;
            for (int i = 1; i < slices; i++)
            {
                real ai = i / real(slices);
                real ang = ai * PI / 2;
                real rr = radius * cos(ang);
                oo.y = radius * sin(ang) + k * ((rr * rr) / (radius * radius));
                VERLIST ee;
                roundedge(ee, oo, vec3::UX, vec3::UZ, rr, slices, 0);

                calcroundnorm(ee, vec3::ZERO);
                if (i == 1)
                    face0(e0, ee);
                else
                    face0(e, ee);
                e = ee;
            }
            bcomvertex = 0;
            vertex top(vec3::UY * radius, vec2(0.5, 0));
            top.n = vec3::UY;
            facepole(e, std::move(top));
            bcomvertex = 1;
        }
    }

    void drawSphereCrown2(float radius,
        float h, // 高度
        float k, // 曲率
        int slices = 18, int stacks = 28) {
       
        bcomvertex = 1;
        VERLIST e0;
        roundedge(e0, vec3::ZERO, vec3::UX, vec3::UZ, radius, slices, 0);
        calcroundnorm(e0, vec3::ZERO);
        {
            real ang0 = asin((radius - h) / radius);
            VERLIST e;
            vec3 oo = vec3::ZERO;
            for (int i = 1; i < slices; i++)
            {
                real ai = i / real(slices);
                real ang = ai * ang0;
                real rr = radius * cos(ang);

                real h_ = h + k * ((rr) / (radius));
                
                oo.y = radius * sin(ang);
                

                VERLIST ee;
                roundedge(ee, oo, vec3::UX, vec3::UZ, rr, slices, 0);
                calcroundnorm(ee, vec3::ZERO);
                if (i == 1)
                    face0(e0, ee);
                else
                    face0(e, ee);
                e = ee;
            }
            bcomvertex = 0;
            vertex top(vec3::UY * h, vec2(0.5, 0));
            top.n = vec3::UY;
            facepole(e, std::move(top));
            bcomvertex = 1;
        }
    }

    void drawSphereCrown(float radius,
        float h, // 高度
        float k, // 曲率
        int slices = 18, int stacks = 28) {

        crd3 c_top = vec3(0,h,0);
       // drawcoord(c_top);

        real ang0 = 2 * atan((h) / radius);
        crd3 c_right = crd3(ang0, vec3::UZ) + vec3(radius, 0, 0);
      //  drawcoord(c_right);

        real H = 1 / tan(ang0) / (0.001 + k);
        crd3 co(-vec3::UY * H);

        VERLIST e;
        Lineiter space = { 0, 1, slices };
        space.walk([&](int i, real t) {
            real a = 0.5 - 0.5 * cos(t * ang0);
            
           
            crd3 g1 = c_top / co;
            crd3 g2 = c_right / co;
            vec3 p = blender::slerp(g1.o, g2.o, t, 1 / (k + 0.1));
            quat q = blender::slerp(g1.Q(), g2.Q(), t);
            crd3 c = coord3(p, q) * co;

            VERLIST ee;
            roundedge(ee, vec3::UY * c.o.y, vec3::UX, vec3::UZ, c.o.x, slices, 0);
            calcroundnorm(ee, vec3::ZERO);

            face0(e, ee);
            e = ee;
            });
    }

    // 绘制圆锥：

    void drawCone(float radius, float height, int slices = 8) {
       // binvnorm = true;
        color = 0xFFFFFFFF;
        VERLIST vertices;
        crd3 c;
        VERLIST e;  TopoLib::roundedge(e, c, radius, slices, 0);
        vertices.push_back(c.o);
        c += vec3::UY * height;
        vertices.push_back(c.o);
        for (int i = 0; i < slices; i++) {
            vertices.push_back(e[i] * cur_coord);
        }
        for (int i = 0; i < slices; i++) {
            int i1 = i + 2;
            int i2 = (i + 1) % slices + 2;
            TopoLib::face(vertices[i1], vertices[i2], vertices[0]);
            TopoLib::face(vertices[i1], vertices[i2], vertices[1]);
        }
        //binvnorm = false;
    }

    // 绘制方盒：

    void drawBox(float width, float height, int length) {
        binvnorm = true;
        cube(vec3::ZERO * cur_coord, cur_coord.ux * width, 1);

        binvnorm = false;
    }
    void drawBox4(float width, float height, int length, int depth) {
        
        // 定义超立方体的顶点坐标
        vec4 vertices[] = {
            {-1.0f, -1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, -1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f, 1.0f}
        };

        // 定义超立方体的边
        GLuint edges[] = {
            0, 1, 2, 3, 4, 5, 6, 7,
            0, 2, 1, 3, 4, 6, 5, 7,
            0, 4, 1, 5, 2, 6, 3, 7
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(4, GL_FLOAT, 0, vertices);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, edges);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    // 绘制圆环：

    void drawTorus(float majorRadius, float minorRadius, int majorSlices = 18, int minorSlices = 18) {
     //binvnorm = true;
        bcomvertex = true;
        VERLIST vertices;
        crd3 c;
        float d_theta = 2 * PI / majorSlices;
        for (int i = 0; i <= majorSlices; i++) {
            float theta = i * d_theta;
            vec3 center = vec3(cos(theta), 0, sin(theta)) * majorRadius;
           // drawcoord(c, center); 
            for (int j = 0; j <= minorSlices; j++) {
                float phi = j * 2 * PI / minorSlices;
                vec3 vertex = center + (c.ux * cos(phi) + c.uy * sin(phi)) * minorRadius;
                vertices.push_back(vertex * cur_coord);
            }
            c.rot(d_theta, vec3::UY);
        }
        color = 0xFFFFFFFF;
        for (int i = 0; i < majorSlices; i++) {
            for (int j = 0; j < minorSlices; j++) {
                int i1 = i * (minorSlices + 1) + j;
                int i2 = (i + 1) * (minorSlices + 1) + j;
                int i3 = (i + 1) * (minorSlices + 1) + j + 1;
                int i4 = i * (minorSlices + 1) + j + 1;
                TopoLib::face(vertices[i1], vertices[i2], vertices[i3]);
                TopoLib::face(vertices[i1], vertices[i3], vertices[i4]);
            }
        }
        bcomvertex = false;
     //  binvnorm = false;
    }

    // 绘制扇形：

    void drawSector(float radius, float angle, int slices = 8) {
        binvnorm = true;
        color = 0xFFFFFFFF;
        VERLIST vertices;
        vertices.push_back(cur_coord.o);
        for (int i = 0; i <= slices; i++) {
            float theta = i * angle / slices;
            vertices.push_back(cur_coord.o + vec3(cos(theta), sin(theta), 0) * radius);
        }
        for (int i = 0; i < slices; i++) {
            TopoLib::face(vertices[0], vertices[i + 1], vertices[i + 2]);
        }
        binvnorm = false;
    }

    void drawPipe(float radius, float length) {
        binvnorm = true;
        color = 0xFFFFFFFF;
        crd3 c;
        VERLIST e1;  TopoLib::roundedge(e1, c, radius, 8, 0);
        c += vec3::UY * length;
        VERLIST e2;  TopoLib::roundedge(e2, c, radius, 8, 0);

        for (auto& v : e1)
            v *= cur_coord;
        for (auto& v : e2)
            v *= cur_coord;

        Lineiter space = { 0, 1, 2 };
        space.walk([&e1, &e2](int i, real x) {
            face(e1, e2);
            });
        binvnorm = false;
    }

    // 绘制弯头：

    void drawWantou0(float majorRadius, float minorRadius1, float minorRadius2, float arcAngle, int majorSlices = 18, int minorSlices = 18) {
        //binvnorm = true;
        VERLIST vertices;
        crd3 c1;
        crd3 c2(arcAngle, vec3::UY);
        float d_theta = arcAngle / majorSlices;
        for (int i = 0; i <= majorSlices; i++) {
            real ai = i / real(majorSlices);
            float theta = i * d_theta;
            vec3 center = cur_coord.o + vec3(cos(theta), 0, sin(theta)) * majorRadius;
            crd3 c = blender::slerp(c1, c2, ai);
            drawcoord(c, center);
            for (int j = 0; j <= minorSlices; j++) {
                float phi = j * 2 * PI / minorSlices;
                vec3 vertex = center + (c.ux * cos(phi) + c.uy * sin(phi)) * blend(minorRadius1, minorRadius2, ai);
                vertices.push_back(vertex);
            }
        }
        color = 0xFFFFFFFF;
        for (int i = 0; i < majorSlices; i++) {
            for (int j = 0; j < minorSlices; j++) {
                int i1 = i * (minorSlices + 1) + j;
                int i2 = (i + 1) * (minorSlices + 1) + j;
                int i3 = (i + 1) * (minorSlices + 1) + j + 1;
                int i4 = i * (minorSlices + 1) + j + 1;
                TopoLib::face(vertices[i1], vertices[i2], vertices[i3]);
                TopoLib::face(vertices[i1], vertices[i3], vertices[i4]);
            }
        }
        //  binvnorm = false;
    }
    void pip_wantou2(crvec p1, crvec v1, crvec p2, crvec v2, real r1, real r2)
    {
        int     majorSlices = 18;
        int     minorSlices = 8;

        VERLIST vertices;
        coord3  c1, c2;
        c1.from_vecs_R(c1.uy, -v1); c1.o = p1;
        c2.from_vecs_R(c2.uy, v2); c2.o = p2;
        drawcoord(c1); drawcoord(c2);
        // 关联不同的约束与参数空间
        Lineiter({ 0, 1, majorSlices + 1 }).walk([&](int i, float theta) {
            real ai = i / real(majorSlices);

            coord3 c = blender::slerp_conjcopy(c1, c2, ai);
            drawcoord(c);
            real r = blend(r1, r2, ai);
            Lineiter({ 0, 2 * PI, minorSlices + 1 }).walk([&](int j, float phi) {
                vec3 vertex = c.o + (c.ux * cos(phi) + c.uz * sin(phi)) * r;
               // pt3d(c.o);
                vertices.push_back(vertex);
                });
            });
        for (int i = 0; i < majorSlices; i++)
        {
            for (int j = 0; j < minorSlices; j++)
            {
                int i1 = i * (minorSlices + 1) + j;
                int i2 = (i + 1) * (minorSlices + 1) + j;
                int i3 = (i + 1) * (minorSlices + 1) + j + 1;
                int i4 = i * (minorSlices + 1) + j + 1;
                TopoLib::face(vertices[i1], vertices[i2], vertices[i3]);
                TopoLib::face(vertices[i1], vertices[i3], vertices[i4]);
            }
        }
    }

    // 弯头面旋转
    void _rotedge_wantou2(const VERLIST& e, real ang, const vec3& o, const vec3& ax, VERLIST& eo)
    {
        for (int i = 0; i < e.size(); i++)
        {
            vertex v = e[i];
            v.p = (e[i].p - o) * quat(ang, ax) + o;
            v.ind = -1;
            eo.PUSH(v);
        }
    }

    // 弯头
    void pip_wantou(crvec p1, crvec p2, real r1, real r2)
    {
        VERLIST e;
        // 3D方向
        vec3 o = vec3::ZERO;
        vec3 uy = (o - p1).normcopy();
        vec3 ux, uz;
        vec3 n = vec3::UZ;
        uz = n;
        ux = uz.cross(uy); ux.norm();

        // 夹角与旋转轴
        vec3 v1 = -p1.normcopy();
        vec3 v2 = p2.normcopy();
        //vec3 n = v1.cross(v2).normcopy();
     
        real ang12 = vv_angle(v1, v2, n);

        //PRINT("ang12=" << ang12);

        real d12 = (p1 - p2).len();
        real R = d12 / sqrt(2 * (1 - cos(ang12)));

        vec3 vv = p1.cross(n).normcopy();
        vec3 _o = p1 + vv * R;

        //drawcoord(c0);

        roundedge(e, p1, ux, uz, r1, 18, 0);
        binvnorm = 0;
        //setnorm(e, -vec3::UY);
       // face(e, getedgecenter(e));
        clearedgeind(e);
        binvnorm = 0;
       // calcroundnorm(e, getedgecenter(e));
        // pipgu(sm, e, r1 * 0.5, true);
        real dis = (p1 - p2).len() * 1.414;
        int  len = 10;
        for (int i = 0; i < len; i++)
        {
            real ai = i / real(len);
            real r = blend(r1, r2, ai);

            VERLIST ee;
            _rotedge_wantou2(e, ang12 / len, _o, n, ee);
            radedge(ee, r);
           // calcroundnorm(ee, getedgecenter(ee));
            //drawcoord(c0);

            face0(ee, e);
            e = ee;
        }
        binvnorm = 1;
        clearedgeind(e);
       // face(e, getedgecenter(e));
        binvnorm = 0;
    }
    // 双倍
    void doublevnum_fang(VERLIST& e, int n)
    {
        VERLIST te;
        te.PUSH(e[0]);
        for (int i = 1; i < n; i++)
        {
            te.PUSH(e[i]);
            te.PUSH(e[i]);
        }
        te.PUSH(e[e.size() - 1]);
        e = te;
    }
    // 方形弯头
    void pip_wantou_fang(crvec p1, crvec p2, real r1, real r2)
    {
        VERLIST e;

        // 3D方向
        vec3 o = vec3::ZERO;
        vec3 uy = (o - p1).normcopy();
        vec3 ux, uz;
        v2vxvy(uy, ux, uz);

        // 夹角与旋转轴
        vec3 v1 = -p1.normcopy();
        vec3 v2 = p2.normcopy();
        vec3 n = v1.cross(v2).normcopy();
        real ang12 = acos(v1.dot(v2));

        real d12 = (p1 - p2).len();
        real R = d12 / sqrt(2 * (1 - cos(ang12)));

        vec3 vv = p1.cross(n).normcopy();
        vec3 _o = p1 + vv * R;

        roundedge(e, p1, ux * r1, uz * r2, 1, 4, PI / 4);
        binvnorm = 1;
        face(e, getedgecenter(e, 4));
        clearedgeind(e);
        binvnorm = 1;
        doublevnum_fang(e, 4);
        crd3 c0;
        {// 法线
            e[0].n = uz;  e[1].n = uz;  
            e[2].n = -ux; e[3].n = -ux; 
            e[4].n = -uz; e[5].n = -uz; 
            e[6].n = ux;  e[7].n = ux;  
        }
        // pipgu(sm, e, r1 * 0.5, true);
        real dis = (p1 - p2).len() * 1.414;
        int  len = 10;
        for (int i = 0; i < len; i++)
        {
            real ai = i / real(len);
            // real r  = blend(r1, r2, ai);

            VERLIST ee;
            rotedge_wantou0(e, ang12 / len, _o, n, ee);
            // radedge(ee, r);
            // calcroundnorm(sm, ee, _o);
            face0(ee, e);
            e = ee;
        }
        binvnorm = 0;
        clearedgeind(e);
        vertex top = getedgecenter(e, 8);
        top.n = p2.normcopy();
        c0.o = top; drawcoord(c0);
        setedgenorm(e, p2.normcopy());
        
        face(e, top);
    }
    void drawWantou(float majorRadius, float minorRadius1, float minorRadius2, float arcAngle, int majorSlices = 18, int minorSlices = 18) {
      
        bcomvertex = true;
        //binvnorm = true;
        VERLIST vertices;
        crd3 c1, c2(arcAngle, vec3::UY);
        // 关联不同的约束与参数空间
        Lineiter({ 0, arcAngle, majorSlices + 1 }).walk([ &](int i, float theta)
            {
                real ai = i / real(majorSlices);
                vec3 center = cur_coord.o + vec3(cos(theta), 0, sin(theta)) * majorRadius;
                crd3 c = blender::slerp(c1, c2, ai);
                drawcoord(c, center);
                real r = blend(minorRadius1, minorRadius2, ai);
                Lineiter({ 0, 2 * PI, minorSlices + 1 }).walk([&](int j, float phi) {
                    vec3 vertex = center + (c.ux * cos(phi) + c.uy * sin(phi)) * r;
                    vertices.push_back(vertex);
                    });
             });
        color = 0xFFFFFFFF;
        for (int i = 0; i < majorSlices; i++) {
            for (int j = 0; j < minorSlices; j++) {
                int i1 = i * (minorSlices + 1) + j;
                int i2 = (i + 1) * (minorSlices + 1) + j;
                int i3 = (i + 1) * (minorSlices + 1) + j + 1;
                int i4 = i * (minorSlices + 1) + j + 1;
                TopoLib::face(vertices[i1], vertices[i2], vertices[i3]);
                TopoLib::face(vertices[i1], vertices[i3], vertices[i4]);
            }
        }
        //  binvnorm = false;
        bcomvertex = false;
    }

    // 绘制曲线：
    void draw3DCurve(crcd3 c1, crcd3 c2) {

        Lineiter({ 0, 1, 18 }).walk([&](int i, real x) {
            crd3 c = blender::lerp(c1, c2, x);
            quat q1(-PI / 2, vec3::UY), q2(PI / 2, vec3::UZ);
            quat  q = quat::slerp(q1, q2, x);
            c *= q;
            pt3d((c * cur_coord).o, 0.1);
            });
    }
    void draw3DCurve(crcd3 c1, crcd3 c2, crcd3 c3) {
        drawcoord(c1); drawcoord(c2); drawcoord(c3);
        Lineiter({ 0, 1, 18 }).walk([&](int i, real x) {
            crd3 c = blender::slerp(c1, c2, x);
            pt3d((c * cur_coord).o, 0.1);
            });

        Lineiter({ 0, 1, 18 }).walk([&](int i, real x) {
            crd3 c = blender::slerp(c2, c3, x);
            pt3d((c * cur_coord).o, 0.1);
            });
    }
    void draw3DSurface(crcd3 c1, crcd3 c2, crcd3 c3, crcd3 c4) {
        VERLIST e1;
        Lineiter({ 0, 1, 18 }).walk([&](int i, real x) {
            crd3 c = blender::lerp(c1, c2, x);
            quat q1(-PI / 2, vec3::UY), q2(PI / 2, vec3::UZ);
            quat  q = quat::slerp(q1, q2, x);
            c *= q;
            pt3d((c * cur_coord).o, 0.1);
            e1.push_back((c * cur_coord).o);
            });
        VERLIST e2;
        Lineiter({ 0, 1, 18 }).walk([&](int i, real x) {
            crd3 c = blender::lerp(c3, c4, x);
            quat q1(-PI / 2, vec3::UY), q2(PI / 2, vec3::UZ);
            quat  q = quat::slerp(q1, q2, x);
            c *= q;
            pt3d((c * cur_coord).o, 0.1);
            e2.push_back((c * cur_coord).o);
            });
        face(e1, e2);
    }
    // 给定三个坐标系定位
    void drawRibbon(crd3 c1, crd3 c2, crd3 c3, real d) 
    {    
        vec3 v21 = vec3(c1 - c2).normalized();
        vec3 v23 = vec3(c3 - c2).normalized();

        real ang = vv_angle_crossdot(v21, v23, c2.uy);
        real delta = d / tan(ang / 2);
        vec3 o = c2.o + (v21 + v23).normcopy()* (d / sin(ang / 2));

        pt3d(o);

        color = 0xFFFF00FF;
        ptr(c1.o, c1.o - v21, 0.05);
        ptr(c3.o, c3.o - v23, 0.05);
        color = 0Xffffffff;

        // 以下的步骤是为了让三个坐标系对齐各自的条带UV以及法向
        quat q;
        q.fromvectors(c1.uz, -v21);

        c1.ucoord( // 设置坐标系的旋转
            c1.Q() // 坐标系转化为四元数
            * q
        );

        q.fromvectors(c3.uz, -v23); // 通过两个向量之间的旋转 构造一个四元数 
        c3.ucoord(c3.Q() * q);

        // 坐标系可视化
        drawcoord(c1); drawcoord(c2); drawcoord(c3);

        ptr(c1, c2, 0.05);
        ptr(c2, c3, 0.05);

        ptr(c1 + c1.ux * d, c2 + c2.ux * d, 0.05);
        ptr(c2 + c2.ux * d, c3 + c3.ux * d, 0.05);

        ptr(c1 - c1.ux * d, c2 - c2.ux * d, 0.05);
        ptr(c2 - c2.ux * d, c3 - c3.ux * d, 0.05);

    }
    void drawBezierCurve(vector<vec3>& c, real t1, real t2, int steps)
    {
        vec3 p0;
        Lineiter({ t1, t2, steps }).walk([&](int i, real x) {
            vec3 p = curve_math::bezier_curve(x, c);
            if (i > 0) {
                ptr(p0, p, 0.1);
            }
            p0 = p;
            });
    }
    void draw3DCurve() {
        crd3 crd_set[] = {
             crd3(0.0f, 0.0f, 0),
             crd3(1.0f, 1.0f, 1),
             crd3(2.0f, 2.0f, 1),
             crd3(3.0f, 3.0f, 2)
        };
        color = 0xFFFFFFFF;
        
        Lineiter({ 0, 1, 18 }).walk([&](int i, real x) {
            crd3 c = blender::lerp(crd_set[0], crd_set[3], x);
            quat q1(-PI / 2, vec3::UY), q2(PI / 2, vec3::UZ);
            quat  q = quat::slerp(q1, q2, x);
            c *= q;
            pt3d((c * cur_coord).o, 0.1);
            });
    }
    void drawLoft(const VERLIST2& shape1, const VERLIST2& shape2, crd3 ce, const VERLIST& path) {
        VERLIST e0;
        vec3 v12;
        for (int i = 0; i < path.size(); i++)
        {
            real t = real(i) / path.size();
            crvec p1 = path[i];
            pt3d(p1, 0.1);
            ce.o = p1;
            if (i + 1 < path.size())
            {
                crvec p2 = path[i + 1];
                v12 = (p2 - p1).normcopy();
            }
            ce.rot(quat(ce.uz, v12));
            drawcoord(ce);
            VERLIST e;
            
            auto shape = blend_shapes(shape1, shape2, t);
            shape_poly(shape, ce, e); closedge(e);
            if(i > 0)
                face0(e0, e);
            e0 = e;
        }
    }

    // 三通

    void draw3DCurve_cylinder(crcd3 c1, rcd3 c2) {
        c2.ucoord(ucoord3(vec3::UY, -vec3::UZ, -vec3::UX));
        drawcoord(c1); drawcoord(c2);

        crd3 oxy = crd3(ucoord3(vec3::UX, vec3::UY, vec3::ZERO), vec3(-1, 0, 0));
        crd3 ozy = crd3(ucoord3(vec3::ZERO, vec3::UY, vec3::UZ), vec3(0, 0, 1));
        {
            VERLIST e1;
            Lineiter({ 0, 1, 18 }).walk([&](int i, real x) {
                crd3 cx = blender::slerp(c1, c2, oxy, (x));
                crd3 cy = blender::slerp(c1, c2, ozy, (x));

                vec3 p = cx.o + cy.o;
                p.y /= 2;
                {
                    pt3d((cx).o, 0.05);
                    pt3d((cy).o, 0.05);
                    pt3d(p, 0.05);
                    e1.push_back(p);
                }
                });


            VERLIST e2;
            arcedge(e2, c1 - vec3::UX - vec3::UZ, 1, 18, 0, PI / 2);
            for (auto& v : e2)
                pt3d(v, 0.05);
            binvnorm = 1;
            face0(e1, e2);
            binvnorm = 0;

            VERLIST e3;
            arcedge(e3, c1 * quat(-PI / 2, vec3::UY) + vec3::UX + vec3::UZ, 1, 18, 0, PI / 2);
            for (auto& v : e3)
                pt3d(v, 0.05);
           
            face0(e1, e3);
        }
    }

    void drawTee() {
       
        vec3 p_set[] = {
             vec3(0.0f, 0.0f, 0),
             vec3(-1.0f, 0.0f, 0),
             vec3(-1.0f, 3.0f, 0),
             vec3(0.0f, 1.0f, -1),
             vec3(0.0f, 1.0f, 0),
             vec3(0.0f, 2.0f, 0),
             vec3(0.0f, 3.0f, -1),
             vec3(1.0f, 0.0f, 0),   
             vec3(0.0f, 2.0f, 0),
             vec3(0.0f, 3.0f, -1),
             vec3(1.0f, 0.0f, 0),
             vec3(1.5f, 3.0f, 0)
        };
        // 三通拓扑: te1->te2->te3 管口平行边，其中te2跟一个孔(te22)相连接
        topoE te1 =     { "e1",{1, 2} }; 
        topoE te2 =     { "e2" };
        topoE te21 =    { "e21",{3,4} };
        topoE te22 =    { "e22", true };
        topoE te221 =   { "e221",{4,5} };
        topoE te222 =   { "e222",{4,5} };
        topoE te23 =    { "e23",{5,6} };
        topoE te3 =     { "e3",{7, 8} };
        {
            te22.addchild(&te221);
            te22.addchild(&te222);
            
            te2.addchild(&te21);
            te2.addchild(&te22);
            te2.addchild(&te23);

            te1.space.steps = te3.space.steps = te2.get_space_stepsL();
        }
        VERLIST e1, e21, e22, e2, e3;
        { 
            coord3 c(-vec3::UY, vec3::UX, vec3::UZ); c.o = (p_set[1] + p_set[2]) / 2;
            Lineiter({ 0, 2 * PI, 12 + 1 }).walk([&](int j, float phi) {
                vec3 vertex = c.o + (c.ux * cos(phi) + c.uz * sin(phi)) * 0.5;
                e1.push_back(vertex);
                pt3d(vertex);
                });
        }
        {// 孔洞
            TopoLib::topo_edge_holeL(e21, te2, p_set);
            TopoLib::topo_edge_holeR(e22, te2, p_set);
            {
                coord3 c(-vec3::UY, vec3::UX, vec3::UZ); c.o = (p_set[3] + p_set[6]) / 2;
                Lineiter({ 0, 2 * PI, (int)e21.size() }).walk([&](int j, float phi) {
                    e21[j].p.y = (vec2(0,0.5).rotcpy(2 * PI / 12 * (4-j))).x + 1.5;
                    e21[j].p.z = (vec2(0,0.5).rotcpy(2 * PI / 12 * (4-j))).y;
                    color = 0xFF00FF00;
                    pt3d(e21[j].p, 0.1);
                    
                   
                    e22[j].p.y = (vec2(0, 0.5).rotcpy(2 * PI / 12 * (4-j))).x + 1.5;
                    e22[j].p.z = (vec2(0, 0.5).rotcpy(2 * PI / 12 * (4-j))).y;
                    color = 0xFF0000FF;
                    pt3d(e22[j].p, 0.1);
                    color = 0xFFFFFFFF;
                });
            }

            //{// 孔洞内边
            //    TopoLib::topo_edge_holeL(e2L, te22, p_set);
            //    e2.insert(e2.end(), e2L.rbegin(), e2L.rend());

            //    TopoLib::topo_edge_holeR(e2R, te22, p_set);
            //    e2.insert(e2.end(), e2R.rbegin(), e2R.rend());
            //}

            for (auto it = e21.begin(); it != e21.end(); it ++ )
            {
                if ((*it).tag == (int64)&te221)
                {
                    e2.push_back((*it));
                }
               
            }
            for (auto it = e22.rbegin() + 1; it != e22.rend(); it++)
            {
                if ((*it).tag == (int64)&te222)
                {
                    e2.push_back((*it));
                }
              /*  else if (e2.size() == 8)
                {
                    e2.push_back((*it));
                    pt3d(*it, 0.25);
                    break;
                }*/
            }
            closedge(e2);
        }
        {
            coord3 c = { {(p_set[7] + p_set[8]) / 2},{ -vec3::UY, vec3::UX, vec3::UZ} };
            Lineiter({ 0, 2 * PI, 12 + 1 }).walk([&](int j, float phi) {
                vec3 vertex = c.o + (c.ux * cos(phi) + c.uz * sin(phi)) * 0.5;
                e3.push_back(vertex);
                pt3d(vertex);
                });
        }
        // 绘制
        binvnorm = true;
        face0(e1, e21); face0(e22, e3);
        extrudedgex(e2, 2);
        binvnorm = false;
    }
}

coord3 pipe_wantou_ani(coord3 c, crvec p1, crvec p2, float r1, float r2, float t = 1)
{
	const float c_utils_size = 0.5; // 纹理坐标U的重复单元距离
	bcomvertex = 1;
	// 方向
	vec3 uy = (p2 - p1).normcopy();
	vec3 ux, uz;
	v2vxvy(uy, ux, uz);

     drawcoord(c);

    VERLIST e0;
     coord3 nc(ux, uy, uz);
     nc.o = p1;
     drawcoord(nc);
	{
		quat q; q.fromvectors(nc.uy, c.uy);
		c = nc * q;

		float r = blend(r1, r2, t);
		vec3 pp = blend(p1, p2, t);
		e0.clear();
		roundedge(e0, c.o, c.ux, c.uz, r, 24, 0);
        drawcoord(c);
	}	

	VERLIST e;
	float r = blend(r1, r2, t);
	vec3 pp = blend(p1, p2, t);
	roundedge(e, p1, ux, uz, r, 24, 0);
	
	calcroundnorm(e, p1);
	if(!e0.empty())
	{
		face(e0, e);
	}
	clearedgeind(e);

	VERLIST ee = e;
	movedge(ee, pp - p1);

	float u = (pp - p1).len() / c_utils_size;

	face(e, ee);
		
	clearedgeind(ee);

    nc.o = p2;
	return nc;
}