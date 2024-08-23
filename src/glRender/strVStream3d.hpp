/** 字符串格式的流式3D向量绘图

        "color 255,255,0",
        "loc 1,1,1",
        "sphere 1",
*/
// -----------------------------------------------
// 字符串格式的流式3D向量绘图
// -----------------------------------------------
extern std::vector<std::pair<std::string, std::string> > msgbuffer;
namespace strVSteam3d
{
    void parse_command(const std::string& command)
    {
        std::istringstream iss(command);
        std::string cmd, params;
        std::getline(iss, cmd, ' ');
        static std::string x_str = "0", y_str = "0", z_str = "0";
        static std::string rx_str = "0", ry_str = "0", rz_str = "0";
        if (cmd == "color" || cmd == "rgb")
        {
            int r, g, b;
            std::string r_str, g_str, b_str;
            std::getline(iss, r_str, ',');
            std::getline(iss, g_str, ',');
            std::getline(iss, b_str, ',');
            r = std::stoi(r_str);
            g = std::stoi(g_str);
            b = std::stoi(b_str);

            // 存储颜色值
            glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
        }
        else if (cmd == "loc")
        {
            std::getline(iss, x_str, ',');
            std::getline(iss, y_str, ',');
            std::getline(iss, z_str, ',');

            shape3d::cur_coord.o.x = std::stof(x_str);
            shape3d::cur_coord.o.y = std::stof(y_str);
            shape3d::cur_coord.o.z = std::stof(z_str);

            PRINTV(shape3d::cur_coord.o.y);
        }
        else if (cmd == "rot")
        {
            std::getline(iss, rx_str, ',');
            std::getline(iss, ry_str, ',');
            std::getline(iss, rz_str, ',');

            vec3 eu;
            eu.x = std::stof(rx_str);
            eu.y = std::stof(ry_str);
            eu.z = std::stof(rz_str);
            shape3d::cur_coord.fromquat(quat(eu.x, eu.y, eu.z));
        }
        else if (cmd == "vec")
        {
            string name_str;
            std::getline(iss, name_str, ' ');
            std::getline(iss, x_str, ',');
            std::getline(iss, y_str, ',');
            std::getline(iss, z_str, ',');
            vec3 v;
            v.x = std::stof(x_str);
            v.y = std::stof(y_str);
            v.z = std::stof(z_str);
            GTable::vec3map[name_str] = v;
        }
        else if (cmd == "crd")
        {
            string name_str;
            std::getline(iss, name_str, ' ');
            std::getline(iss, x_str, ',');
            std::getline(iss, y_str, ',');
            std::getline(iss, z_str, ',');
            vec3 v;
            v.x = std::stof(x_str);
            v.y = std::stof(y_str);
            v.z = std::stof(z_str);
            vec3 eu;
            std::getline(iss, rx_str, ',');
            std::getline(iss, ry_str, ',');
            std::getline(iss, rz_str, ',');
            eu.x = std::stof(rx_str) * PI / 180;
            eu.y = std::stof(ry_str) * PI / 180;
            eu.z = std::stof(rz_str) * PI / 180;
            GTable::coord3map[name_str] = coord3(v.x, v.y, v.z, eu.x, eu.y, eu.z);
        }
        else if (cmd == "wantou_ani")
        {
            // 解析 wantou_ani x,y,z rx,ry,rz p1.x,p1.y,p1.z p2.x,p2.y,p2.z r1 r2 t
            // 调用 pipe_wantou_ani(coord3 c, crvec p1, crvec p2, float r1, float r2, float t = 1)
            std::getline(iss, x_str, ',');
            std::getline(iss, y_str, ',');
            std::getline(iss, z_str, ' ');
            vec3 v;
            v.x = std::stof(x_str);
            v.y = std::stof(y_str);
            v.z = std::stof(z_str);
            vec3 eu;
            std::getline(iss, rx_str, ',');
            std::getline(iss, ry_str, ',');
            std::getline(iss, rz_str, ' ');
            eu.x = std::stof(rx_str) * PI / 180;
            eu.y = std::stof(ry_str) * PI / 180;
            eu.z = std::stof(rz_str) * PI / 180;

            string px_str,py_str,pz_str;
            std::getline(iss, px_str, ',');
            std::getline(iss, py_str, ',');
            std::getline(iss, pz_str, ' ');
            vec3 p1;
            p1.x = std::stof(px_str);
            p1.y = std::stof(py_str);
            p1.z = std::stof(pz_str);
            std::getline(iss, px_str, ',');
            std::getline(iss, py_str, ',');
            std::getline(iss, pz_str, ' ');
            vec3 p2;
            p2.x = std::stof(px_str);
            p2.y = std::stof(py_str);
            p2.z = std::stof(pz_str);

            string r1_str,r2_str,t_str;
            std::getline(iss, r1_str, ' ');
            std::getline(iss, r2_str, ' ');
            float r1 = std::stof(r1_str);
            float r2 = std::stof(r2_str);
            std::getline(iss, t_str, ' ');
            float t = std::stof(t_str);

            pipe_wantou_ani(crd3(v.x,v.y,v.z,eu.x,eu.y,eu.z), p1, p2, r1, r2, t);
        }
        else if (cmd == "phg")
        {
            string phg;
            std::getline(iss, phg, ' ');
            doPHG(phg.c_str());

            PM::setup_mesh_data();
            substage = "phg";
            glutPostRedisplay();
        }
        else
        {
            string shape = STR::toLower(cmd);
            if (shape == "cylinder")
            {
                float x = std::stof(x_str);
                float y = std::stof(y_str);
                float z = std::stof(z_str);

                std::string radius_str;
                std::getline(iss, radius_str, ' ');
                float radius = std::stof(radius_str);

                std::string height_str;
                std::getline(iss, height_str, ' ');
                float height = std::stof(height_str);
                //shape3d::cur_coord.o = vec3(x, y, z);
                shape3d::drawCylinder(radius, height);
            }
            else if (shape == "sphere")
            {
                std::string radius_str;
                std::getline(iss, radius_str, ' ');
                float radius = std::stof(radius_str);

                shape3d::drawSphere(radius);
            }
            else if (shape == "spherecrown")
            {
                std::string radius_str;
                std::getline(iss, radius_str, ' ');
                float radius = std::stof(radius_str);

                std::string height_str;
                std::getline(iss, height_str, ' ');
                float height = std::stof(height_str);

                std::string curvature_str;
                std::getline(iss, curvature_str, ' ');
                float curvature = std::stof(curvature_str);

                shape3d::drawSphereCrown(radius, height, curvature);
            }
            else if (shape == "cone")
            {
                float x = std::stof(x_str);
                float y = std::stof(y_str);
                float z = std::stof(z_str);

                std::string radius_str;
                std::getline(iss, radius_str, ' ');
                float radius = std::stof(radius_str);

                std::string height_str;
                std::getline(iss, height_str, ' ');
                float height = std::stof(height_str);

                //shape3d::cur_coord.o = vec3(x, y, z);
                shape3d::drawCone(radius, height);
            }
            else if (shape == "torus")
            {
                float x = std::stof(x_str);
                float y = std::stof(y_str);
                float z = std::stof(z_str);
                //shape3d::cur_coord.o = vec3(x, y, z);
                std::string inner_radius_str;
                std::getline(iss, inner_radius_str, ' ');
                float inner_radius = std::stof(inner_radius_str);

                std::string outer_radius_str;
                std::getline(iss, outer_radius_str, ' ');
                float outer_radius = std::stof(outer_radius_str);

                shape3d::drawTorus(inner_radius, outer_radius);
            }
            else if (shape == "pipe")
            {
                float x = std::stof(x_str);
                float y = std::stof(y_str);
                float z = std::stof(z_str);
                shape3d::cur_coord.o = vec3(x, y, z);
                std::string radius_str;
                std::getline(iss, radius_str, ' ');
                float radius = std::stof(radius_str);

                std::string height_str;
                std::getline(iss, height_str, ' ');
                float height = std::stof(height_str);
                shape3d::cur_coord.o = vec3(x, y, z);
                shape3d::drawPipe(radius, height);
            }
            else if (shape == "reducer")
            {
                //shape3d::cur_coord.o = vec3(x, y, z);
                std::string major_radius_str;
                std::getline(iss, major_radius_str, ' ');
                float major_radius = std::stof(major_radius_str);

                std::string minor_radius1_str;
                std::getline(iss, minor_radius1_str, ' ');
                float minor_radius1 = std::stof(minor_radius1_str);

                /* std::string minor_radius2_str;
                 std::getline(iss, minor_radius2_str, ' ');
                 float minor_radius2 = std::stof(minor_radius2_str);*/

                std::string arc_angle_str;
                std::getline(iss, arc_angle_str, ' ');
                float arc_angle = std::stof(arc_angle_str);

                shape3d::drawWantou(major_radius, minor_radius1, minor_radius1, arc_angle);
            }
            else if (shape == "sector")
            {
                float x = std::stof(x_str);
                float y = std::stof(y_str);
                float z = std::stof(z_str);
                //shape3d::cur_coord.o = vec3(x, y, z);
                std::string radius_str;
                std::getline(iss, radius_str, ' ');
                float radius = std::stof(radius_str);

                std::string angle_str;
                std::getline(iss, angle_str, ' ');
                float angle = std::stof(angle_str);

                shape3d::drawSector(radius, angle);
            }
            else if (shape == "box" || shape == "cube")
            {
                //shape3d::cur_coord.o = vec3(x, y, z);
                std::string width_str;
                std::getline(iss, width_str, ' ');
                float width = std::stof(width_str);

                std::string height_str;
                std::getline(iss, height_str, ' ');
                float height = std::stof(height_str);

                std::string length_str;
                std::getline(iss, length_str, ' ');
                float length = std::stof(length_str);

                shape3d::drawBox(width, height, length);
            }
            else if (shape == "box4" || shape == "cube4")
            {
                //shape3d::cur_coord.o = vec3(x, y, z);
                std::string width_str;
                std::getline(iss, width_str, ' ');
                float width = std::stof(width_str);

                std::string height_str;
                std::getline(iss, height_str, ' ');
                float height = std::stof(height_str);

                std::string length_str;
                std::getline(iss, length_str, ' ');
                float length = std::stof(length_str);

                std::string depth_str;
                std::getline(iss, depth_str, ' ');
                float depth = std::stof(depth_str);

                shape3d::drawBox4(width, height, length, depth);
            }
            else if (shape == "curve")
            {
                std::string c1, c2;
                std::getline(iss, c1, ' ');
                std::getline(iss, c2, ' ');
                shape3d::draw3DCurve(GTable::coord3map[c1], GTable::coord3map[c2]);
            }
            else if (shape == "curve_cylinder")
            {
                std::string c1, c2;
                std::getline(iss, c1, ' ');
                std::getline(iss, c2, ' ');
                shape3d::draw3DCurve_cylinder(GTable::coord3map[c1], GTable::coord3map[c2]);
            }
            else if (shape == "ribbon")
            {
                std::string c1, c2, c3, d;
                std::getline(iss, c1, ' ');
                std::getline(iss, c2, ' ');
                std::getline(iss, c3, ' ');
                std::getline(iss, d, ' ');
                shape3d::drawRibbon(GTable::coord3map[c1], GTable::coord3map[c2], GTable::coord3map[c3], std::stof(d));
            }
            else if (shape == "loft")
            {
                std::string c1;
                std::getline(iss, c1, ' ');

                VERLIST2 e1, e2;
                VERLIST path;
                e1 = { {-1,1},{-1,-1},{1,-1},{1,1} };
                e2 = { {-2,1},{-1,-1},{1,-3},{2,1} };
                path = { {0,0,0},{0,0,1},{0,1,2},{0,1,3} };
                shape3d::drawLoft(e1,e2, GTable::coord3map[c1],path);
            }
            else if(shape == "surface")
            {
                std::string c1, c2, c3, c4;
                std::getline(iss, c1, ' ');
                std::getline(iss, c2, ' ');
                std::getline(iss, c3, ' ');
                std::getline(iss, c4, ' ');
                shape3d::draw3DSurface(GTable::coord3map[c1], GTable::coord3map[c2], GTable::coord3map[c3], GTable::coord3map[c4]);
            }
            else if (shape == "beizercurve")
            {
                std::vector<vec3> controlPoints;
                std::string c1, c2, c3, c4;
                std::getline(iss, c1, ' ');
                std::getline(iss, c2, ' ');
                std::getline(iss, c3, ' ');
                std::getline(iss, c4, ' ');
                controlPoints.push_back(GTable::coord3map[c1]);
                controlPoints.push_back(GTable::coord3map[c2]);
                controlPoints.push_back(GTable::coord3map[c3]);
                controlPoints.push_back(GTable::coord3map[c4]);
                real t1, t2;
                int steps;
                iss >> t1 >> t2 >> steps;
                shape3d::drawBezierCurve(controlPoints, t1, t2, steps);
            }
            else if (shape == "line")
            {
                float x1 = std::stof(x_str);
                float y1 = std::stof(y_str);
                float z1 = std::stof(z_str);

                std::string x2_str;
                std::getline(iss, x2_str, ' ');
                float x2 = std::stof(x2_str);

                std::string y2_str;
                std::getline(iss, y2_str, ' ');
                float y2 = std::stof(y2_str);

                std::string z2_str;
                std::getline(iss, z2_str, ' ');
                float z2 = std::stof(z2_str);

                shape3d::drawLine(x1,y1,z1, x2,y2,z2);
            }
            else if (shape == "triangle")
            {
                std::string x1_str;
                std::getline(iss, x1_str, ' ');
                float x1 = std::stof(x1_str);

                std::string y1_str;
                std::getline(iss, y1_str, ' ');
                float y1 = std::stof(y1_str);

                std::string z1_str;
                std::getline(iss, z1_str, ' ');
                float z1 = std::stof(z1_str);

                std::string x2_str;
                std::getline(iss, x2_str, ' ');
                float x2 = std::stof(x2_str);

                std::string y2_str;
                std::getline(iss, y2_str, ' ');
                float y2 = std::stof(y2_str);

                std::string z2_str;
                std::getline(iss, z2_str, ' ');
                float z2 = std::stof(z2_str);

                std::string x3_str;
                std::getline(iss, x3_str, ' ');
                float x3 = std::stof(x3_str);

                std::string y3_str;
                std::getline(iss, y3_str, ' ');
                float y3 = std::stof(y3_str);

                std::string z3_str;
                std::getline(iss, z3_str, ' ');
                float z3 = std::stof(z3_str);

                TopoLib::face0(vec3(x1, y1, z1), vec3(x2, y2, z2), vec3(x3, y3, z3));
            }
            else if (shape == "poly")
            {
                std::vector<float> vertices;
                std::string vertex_str;
                while (std::getline(iss, vertex_str, ' '))
                {
                    float vertex = std::stof(vertex_str);
                    vertices.push_back(vertex);
                }
                shape3d::drawPolygon(vertices);
            }
        }
    }

    // 绘制命令
    std::vector<std::string> commands;/* = {
            "color 255,255,0",
            "loc 255,255",
            "Cylinder 1,8"
    };*/
    void docmd()
    {
        if (commands.empty())
        {
            return;
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1, 1, 1);

        for (const auto& command : commands)
            if (command.substr(0, 2) != "ui")
                parse_command(command);

        glFlush();
    }
}