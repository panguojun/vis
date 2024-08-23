/**				����2D���ζ���
				todo: ��״ͼ�� ��ѧͼ��
			
*/
// -----------------------------------------------
// primitive shapes
// -----------------------------------------------
namespace shape2d
{
    // ���Ƶ㣺

    void drawPoint(float x, float y, float size) {
        // glTranslatef(x, y, 0.0f);
        glPointSize(size);
        glBegin(GL_POINTS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
    }

    // ���Ƶ㼯��

    void drawPoints(const std::vector<float>& points, float size) {
        glPointSize(size);
        glBegin(GL_POINTS);
        for (int i = 0; i < points.size(); i += 2) {
            glVertex2f(points[i], points[i + 1]);
        }
        glEnd();
    }

    // ����Բ�Σ�

    void drawCircle(float x, float y, float radius, int triangleAmount) {
        // glTranslatef(x, y, 0.0f);
      
        glBegin(GL_TRIANGLE_FAN);
        //radius *= spin::sin(1, 0.1);
        for (int i = 0; i <= triangleAmount; i++) {
            glVertex2f(
                (radius * cos(i * twicePi / triangleAmount)),
                (radius * sin(i * twicePi / triangleAmount))
            );
        }
        glEnd();
    }

    // ���Ʒ��Σ�

    void drawSquare(float x, float y, float width, float height) {
        // glTranslatef(x, y, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-width / 2, -height / 2);
        glVertex2f(width / 2, -height / 2);
        glVertex2f(width / 2, height / 2);
        glVertex2f(-width / 2, height / 2);
        glEnd();
    }

    // ������Ʒ��κ���

    void drawRect(float x, float y, float width, float height)
    {
        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }
    
    // ������������κ���

    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
    {
        glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glEnd();
    }

    // �������ߣ�

    void drawRay(float x, float y, float length) {
        // glTranslatef(x, y, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(length, 0.0f);
        glEnd();
    }

    // ���Ƽ�ͷ��

    void drawArrow(float x, float y, float length, float arrowSize) {
        glRotatef(spin::frame * PI / 8, 0,0,1);
        // glTranslatef(x, y, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(length, 0.0f);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2f(length - arrowSize, arrowSize / 2.0f);
        glVertex2f(length, 0.0f);
        glVertex2f(length - arrowSize, -arrowSize / 2.0f);
        glEnd();
    }

    // ������Բ��

    void drawEllipse(float x, float y, float radiusX, float radiusY, int triangleAmount) {
        // glTranslatef(x, y, 0.0f);
        glColor3f(1.0f, 1.0f, 0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for (int i = 0; i <= triangleAmount; i++) {
            glVertex2f(
                (radiusX * cos(i * twicePi / triangleAmount)),
                (radiusY * sin(i * twicePi / triangleAmount))
            );
        }
        glEnd();
    }

    // ���������ߣ�

    void drawParabola(float x, float y, float a, float b, float c, float halfWidth, float stepSize) {
        // glTranslatef(x, y, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (float i = -halfWidth; i <= halfWidth; i += stepSize) {
            glVertex2f(i, a * i * i + b * i + c);
        }
        glEnd();
    }

    // ���ƽǣ�

    void drawAngle(float x, float y, float angle, float length) {
        // glTranslatef(x, y, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(cos(angle) * length, sin(angle) * length);
        glEnd();
    }

    // ���������ߣ�

    void drawSpiral(float x, float y, float radius, float numLoops, float stepSize) {
        // glTranslatef(x, y, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (float i = 0.0f; i <= numLoops * twoPi; i += stepSize) {
            glVertex2f(i / numLoops * radius * cos(i), i / numLoops * radius * sin(i));
        }
        glEnd();
    }
    // �������ߣ�
    void drawPolyline(const std::vector<vec2>& points) {
        glBegin(GL_LINE_STRIP);
        for (auto point : points) {
            glVertex2f(point.x, point.y);
        }
        glEnd();
    }
    void drawPolyline0(const std::vector<vec2>& points) {
        glBegin(GL_LINES);
        for (auto point : points) {
            glVertex2f(point.x, point.y);
        }
        glEnd();
    }

    // ����ֱ�ߣ�

    void drawLine(float x1, float y1, float x2, float y2) {
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }

    // �������ߣ�

    void drawCurve(const std::vector<vec2>& points) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < points.size() - 1; i++) {
            float x1 = points[i].x;
            float y1 = points[i].y;
            float x2 = points[i + 1].x;
            float y2 = points[i + 1].y;
            float cx = (x1 + x2) / 2;
            float cy = (y1 + y2) / 2;
            glVertex2f(x1, y1);
            for (float t = 0.0; t <= 1.0; t += 0.1) {
                float px = pow(1 - t, 2) * x1 + 2 * (1 - t) * t * cx + pow(t, 2) * x2;
                float py = pow(1 - t, 2) * y1 + 2 * (1 - t) * t * cy + pow(t, 2) * y2;
                glVertex2f(px, py);
            }
        }
        glEnd();
    }

    // ����Բ����

    void drawArc(float x, float y, float radius, float start_angle, float end_angle) {
        // glTranslatef(x, y, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (float angle = start_angle; angle <= end_angle; angle += 0.1) {
            float px = radius * cos(angle * PI / 180);
            float py = radius * sin(angle * PI / 180);
            glVertex2f(px, py);
        }
        glEnd();
    }

    // �߿�

    void drawCADFrame()
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(4.0f);

        // ������߿�
        glBegin(GL_LINES);
        glVertex2f(-1, -1);
        glVertex2f(-1, 1);
        glEnd();

        // �����ұ߿�
        glBegin(GL_LINES);
        glVertex2f(1, -1);
        glVertex2f(1, 1);
        glEnd();

        // �����±߿�
        glBegin(GL_LINES);
        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glEnd();

        // �����ϱ߿�
        glBegin(GL_LINES);
        glVertex2f(-1, 1);
        glVertex2f(1, 1);
        glEnd();
    }
}
