/** 字符串格式的流式2D向量图
        "color 255,255,0",
        "loc 255,255",
        "point 1",
        "points 1 2",
        "circle 50",
        "line 500 0",
        "rect 100 50",
        "arrow 400 400"
        "ellipse 100 50",
        "parabola 800 400 1"
        "angle 100 45 135",
        "spiral 20 200 10",
        "poly 900,900 950,950 1000,900 1050,950 1100,900",
        "curve 1200,1200 1300,1300 1400,1200 1500,1500",
        "arc 100 0 180"
*/
// -----------------------------------------------
// 字符串格式的流式2D向量图
// -----------------------------------------------
namespace strVSteam2d
{
    void parse_command(const std::string& command)
    {
        std::istringstream iss(command);
        std::string cmd, params;
        std::getline(iss, cmd, ' ');
        static std::string x_str = "0", y_str = "0";
        if (cmd == "rgb")
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
        else if (cmd == "scale")
        {
            std::string sx_str, sy_str;
            std::getline(iss, sx_str, ',');
            std::getline(iss, sy_str, ',');

            float x = std::stof(sx_str);
            float y = std::stof(sy_str);

            // 存储颜色值
            glScalef(x, y, 1);
        }
        else if (cmd == "loc")
        {
            std::getline(iss, x_str, ',');
            std::getline(iss, y_str, ',');

            float x = std::stof(x_str);
            float y = std::stof(y_str);
            glTranslatef(x, y, 0.0f);

        }
        else if (cmd == "point")
        {
            std::string tx_str, ty_str;
            std::getline(iss, tx_str, ',');
            std::getline(iss, ty_str, ',');

            float x = std::stof(tx_str);
            float y = std::stof(ty_str);

            shape2d::drawPoint(x, y, 1);
        }
        else
        {
            string shape = cmd;
            if (shape == "points")
            {
                std::vector<float> points;
                std::string point_str;
                while (std::getline(iss, point_str, ' '))
                {
                    float point = std::stof(point_str);
                    points.push_back(point);
                }
                shape2d::drawPoints(points, 1);
            }
            else if (shape == "circle2d")
            {
                std::string radius_str;
                std::getline(iss, radius_str, ' ');
                float radius = std::stof(radius_str);
                shape2d::drawCircle(0, 0, radius, 58);
            }
            else if (shape == "rect")
            {
                std::string width_str, height_str;
                std::getline(iss, width_str, ' ');
                std::getline(iss, height_str, ' ');
                float width = std::stof(width_str);
                float height = std::stof(height_str);
                shape2d::drawRect(0, 0, width, height);
            }
            else if (shape == "square")
            {
                std::string side_str;
                std::getline(iss, side_str, ' ');
                float side = std::stof(side_str);
                shape2d::drawSquare(0, 0, side, side);
            }
            else if (shape == "triangle2d")
            {
                std::string base_str, height_str, x2_str, y2_str, x3_str, y3_str;
                std::getline(iss, base_str, ' ');
                std::getline(iss, height_str, ' ');

                float base = std::stof(base_str);
                float height = std::stof(height_str);

                float x1 = 0.0, y1 = 0.0;
                float x2 = base, y2 = 0.0;
                float x3 = 0.0, y3 = height;

                shape2d::drawTriangle(x1, y1, x2, y2, x3, y3);
            }
            else if (shape == "arrow")
            {
                std::string x2_str, y2_str;
                std::getline(iss, x2_str, ' ');
                std::getline(iss, y2_str, ' ');
                float x1 = std::stof(x_str);
                float y1 = std::stof(y_str);
                float len = std::stof(x2_str);
                float sz = std::stof(y2_str);
                shape2d::drawArrow(x1, y1, len, sz);
            }
            else if (shape == "line2d")
            {
                std::string x1_str, y1_str, x2_str, y2_str;
                std::getline(iss, x1_str, ',');
                std::getline(iss, y1_str, ' ');
                std::getline(iss, x2_str, ',');
                std::getline(iss, y2_str, ' ');

                float x1 = std::stof(x1_str);
                float y1 = std::stof(y1_str);
                float x2 = std::stof(x2_str);
                float y2 = std::stof(y2_str);
                shape2d::drawLine(x1, y1, x2, y2);
            }
            else if (shape == "polyline2d" || shape == "poly" || shape == "polyline")
            {
                std::vector<vec2> points;
                std::string point_str;

                while (std::getline(iss, point_str, ' '))
                {
                    std::istringstream iss_point(point_str);
                    std::string x_str, y_str;
                    std::getline(iss_point, x_str, ',');
                    std::getline(iss_point, y_str, ',');
                    if (x_str.empty() || y_str.empty())
                        break;
                    float x = std::stof(x_str);
                    float y = std::stof(y_str);
                    vec2 point(x, y);
                    points.push_back(point);
                }
                shape2d::drawPolyline(points);
            }
            else if (shape == "curve2d")
            {
                std::string points_str;
                std::getline(iss, points_str, ' ');
                std::vector<vec2> points;
                std::istringstream iss_points(points_str);
                std::string point_str;
                while (std::getline(iss_points, point_str, ';'))
                {
                    std::istringstream iss_point(point_str);
                    std::string x_str, y_str;
                    std::getline(iss_point, x_str, ',');
                    std::getline(iss_point, y_str, ',');
                    vec2 point;
                    point.x = std::stof(x_str);
                    point.y = std::stof(y_str);
                    points.push_back(point);
                }
                shape2d::drawCurve(points);
            }
            else if (shape == "arc")
            {
                std::string radius_str, start_angle_str, end_angle_str;
                std::getline(iss, radius_str, ' ');
                std::getline(iss, start_angle_str, ' ');
                std::getline(iss, end_angle_str, ' ');
                float x = std::stof(x_str);
                float y = std::stof(y_str);
                float radius = std::stof(radius_str);
                float end_angle = std::stof(start_angle_str);
                //float end_angle = std::stof(end_angle_str);
               // shape2d::drawArc(x, y, radius, start_angle, end_angle);
                shape2d::drawArc(x, y, radius, 0, end_angle);
            }
            else if (shape == "ellipse2d")
            {
                std::string radius_x_str, radius_y_str;
                std::getline(iss, radius_x_str, ' ');
                std::getline(iss, radius_y_str, ' ');
                float radius_x = std::stof(radius_x_str);
                float radius_y = std::stof(radius_y_str);
                shape2d::drawEllipse(0, 0, radius_x, radius_y, 58);
            }
            else if (shape == "parabola")
            {
                std::string focus_x_str, focus_y_str, directrix_str;
                std::getline(iss, focus_x_str, ' ');
                std::getline(iss, focus_y_str, ' ');
                std::getline(iss, directrix_str, ' ');
                float focus_x = std::stof(focus_x_str);
                float focus_y = std::stof(focus_y_str);
                float directrix = std::stof(directrix_str);
                shape2d::drawParabola(0, 0, focus_x, focus_y, directrix, 18, 8);
            }
            else if (shape == "angle")
            {
                std::string length_str, angle_str;
                std::getline(iss, length_str, ' ');
                std::getline(iss, angle_str, ' ');
                float x = std::stof(x_str);
                float y = std::stof(y_str);
                float length = std::stof(length_str);
                float angle = std::stof(angle_str);
                shape2d::drawAngle(x, y, length, angle);
            }
            else if (shape == "spiral")
            {
                std::string start_radius_str, end_radius_str, angle_str;
                std::getline(iss, start_radius_str, ' ');
                std::getline(iss, end_radius_str, ' ');
                std::getline(iss, angle_str, ' ');
                float start_radius = std::stof(start_radius_str);
                float end_radius = std::stof(end_radius_str);
                float angle = std::stof(angle_str);
                shape2d::drawSpiral(0, 0, start_radius, end_radius, angle);
            }
            else if (shape == "curve")
            {
                std::vector<vec2> points;
                std::string point_str;
                while (std::getline(iss, point_str, ' '))
                {
                    std::string x_str, y_str;
                    std::istringstream iss2(point_str);
                    std::getline(iss2, x_str, ',');
                    std::getline(iss2, y_str, ',');
                    float x = std::stof(x_str);
                    float y = std::stof(y_str);
                    vec2 point(x, y);
                    points.push_back(point);
                }
                shape2d::drawCurve(points);
            }
        }
    }

    // 绘制命令
    std::vector<std::string> commands;
    void docmd()
    {
       // if (commands.empty())
       //     return;

        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1, 1, 1);
        glPushMatrix();

        glPushMatrix();
        glScalef(SCREENX / 2, SCREENY / 2, 1);
        glTranslatef(1, 1, 0.0f);
        shape2d::drawCADFrame();

       // glScissor(0.02, 0.02, 0.98, 0.98);

        glPopMatrix();
        real scalX = (real)SCREENX / glutGet(GLUT_WINDOW_WIDTH);
        real scalY = (real)SCREENY / glutGet(GLUT_WINDOW_HEIGHT);

        glTranslatef(OFFSET_X * scalX, OFFSET_Y * scalY, 0.0f);

        if(substage == "hlr")
        {// 绘制共享数据
            /*std::vector<vec2>& poly = GTable::poly2fmap["cur"];
            if (!poly.empty())
            {
                glTranslatef(500 + OFFSET_X - poly.front().x * 100.0f, 500 + OFFSET_Y - poly.front().y * 100.0f, 0.0f);
                glScalef(100.0f, 100.0f, 1.0f);

                shape2d::drawPolyline(poly);
            }
            else*/
            {
                hlr_map.clear();
                glDisable(GL_MULTISAMPLE);

                drawhlr();
            }
        }
        else  if (substage == "front")
        {
            front_view();
        }
        else
        {
            glScalef(100.0f * g_hlr_scale.x, 100.0f * g_hlr_scale.y, 1);
            for (const auto& command : commands)
            {
                parse_command(command);
            }

            draw_estack2(); // 2d estack
        }

        glFlush();
        glPopMatrix();
    }

    // ------------------------------------------------
    using json = nlohmann::json;
    // 网络GET
    // ------------------------------------------------
    void on_get(const std::multimap<std::string, std::string>& params, std::string& out)
    {
        json lines;

        auto range = params.equal_range("cmd");
        for (auto it = range.first; it != range.second; ++it)
        {
            const std::string& cmd = it->second;

            if (cmd == "feature")
            {
                for (auto& feature : GTable::featuremap)
                {
                    for (auto& feature_segs : feature.second)
                    {
                        if (feature_segs.empty())
                            continue;

                        for (auto& seg : feature_segs)
                        {
                            vec2 a_(200 + seg.a.p.x * 80, 400 + seg.a.p.y * 80);
                            vec2 b_(200 + seg.b.p.x * 80, 400 + seg.b.p.y * 80);

                            json lineInfo = {
                             { "a", { { "x", a_.x }, { "y", a_.y } } },
                             { "b", { { "x", b_.x }, { "y", b_.y } } }
                            };

                            // Add the line segment object to the array
                            lines.push_back(lineInfo);
                        }
                    }
                }
                out = lines.dump();
            }
            else if (cmd == "feature_svg")
            {
                std::string svg = "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";

                for (auto& feature : GTable::featuremap)
                {
                    for (auto& feature_segs : feature.second)
                    {
                        if (feature_segs.empty())
                            continue;

                        for (auto& seg : feature_segs)
                        {
                            vec2 a_(200 + seg.a.p.x * 80, 400 + seg.a.p.y * 80);
                            vec2 b_(200 + seg.b.p.x * 80, 400 + seg.b.p.y * 80);

                            std::string line = "<line x1=\"" + std::to_string(a_.x) + "\" y1=\"" + std::to_string(a_.y) + "\" x2=\"" + std::to_string(b_.x) + "\" y2=\"" + std::to_string(b_.y) + "\" style=\"stroke:black;stroke-width:2\" />";
                            svg += line;
                        }
                    }
                }

                svg += "</svg>";
                out = svg;
            }
        }
    }
}