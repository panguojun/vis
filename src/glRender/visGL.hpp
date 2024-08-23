#pragma once
// ******************************************************
// GL���ƽӿ�
// ******************************************************
// min draw interface
void pset(crvec p, float size, crvec cor)
{
    glPointSize(size); //���õ�Ĵ�С
    glColor3f(cor.r, cor.g, cor.b); //���õ����ɫ
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
}
void pset(crvec p, crvec cor)
{
    glPointSize(1); //���õ�Ĵ�С
    glColor3f(cor.r, cor.g, cor.b); //���õ����ɫ
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
}
void line(crvec p1, crvec p2, crvec cor)
{
    glColor3f(cor.r, cor.g, cor.b); //���õ����ɫ
    glPushMatrix();

    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
    glPopMatrix();
}
void line(crvec p1, crvec p2, float size, crvec cor)
{
    glPointSize(size); //���õ�Ĵ�С
    glColor3f(cor.r, cor.g, cor.b); //���õ����ɫ
    glPushMatrix();

    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();
    glPopMatrix();
}
void text0(crvec2 p, crstr label)
{
    glColor3f(0.0f, 1.0f, 1.0f);
    glRasterPos2f(p.x + 0.5f * (label.length() - 8), p.y + 0.5f * (10));
    for (int i = 0; i < label.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, label[i]);
    }
}

// ����
void drawPoint3(crvec p, float size, crvec cor)
{
    glPointSize(size); //���õ�Ĵ�С
    glColor3f(cor.r, cor.g, cor.b); //���õ����ɫ
    glBegin(GL_POINTS);
    glVertex3f(p.x, p.y, p.z);
    glEnd();
}
void drawPoint3_cs(crvec p, float size, crvec cor)
{
    glPointSize(size);
    glColor3f(cor.r, cor.g, cor.b);

    glBegin(GL_LINES);
    //����ʮ��
    glVertex3f(p.x - size, p.y, p.z);
    glVertex3f(p.x + size, p.y, p.z);
    glVertex3f(p.x, p.y - size, p.z);
    glVertex3f(p.x, p.y + size, p.z);
    glEnd();

    glBegin(GL_POINTS);
    //����ɢ��
    glVertex3f(p.x, p.y, p.z);
    glEnd();
}
void drawPoint(crvec2 p, float size, crvec cor)
{
    glPointSize(size); //���õ�Ĵ�С
    glColor3f(cor.r, cor.g, cor.b); //���õ����ɫ
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
}
void drawPoint(crvec2 p, crvec cor)
{
    glPointSize(1); //���õ�Ĵ�С
    glColor3f(cor.r, cor.g, cor.b); //���õ����ɫ
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
}
// ���㼯
void drawPoints(const vector<vec3>& points, float size, crvec cor)
{
    glPointSize(size);
    glColor3f(cor.r, cor.g, cor.b);

    glBegin(GL_POINTS);
    for (int i = 0; i < points.size(); i++)
    {
        glVertex3f(points[i].x, points[i].y, points[i].z);
    }
    glEnd();
}
void drawPoints_cs(const vector<vec3>& points, float size, crvec cor)
{
    glColor3f(cor.r, cor.g, cor.b);

    size /= 100;
    glBegin(GL_LINES);
    for (int i = 0; i < points.size(); i++)
    {
        float x = points[i].x;
        float y = points[i].y;
        float z = points[i].z;

        //����ʮ��
        glVertex3f(x - size, y, z);
        glVertex3f(x + size, y, z);
        glVertex3f(x, y - size, z);
        glVertex3f(x, y + size, z);
    }
    glEnd();
}
void drawPoints(const vector<vec2>& points, float size, crvec cor)
{
    glPointSize(size);
    glColor3f(cor.r, cor.g, cor.b);

    glBegin(GL_POINTS);
    for (int i = 0; i < points.size(); i++)
    {
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();
}

void drawPoints_cs(const vector<vec2>& points, float size, crvec cor)
{
    glPointSize(size);
    glColor3f(cor.r, cor.g, cor.b);

    glBegin(GL_LINES);
    for (int i = 0; i < points.size(); i++)
    {
        float x = points[i].x;
        float y = points[i].y;

        //����ʮ��
        glVertex2f(x - size, y);
        glVertex2f(x + size, y);
        glVertex2f(x, y - size);
        glVertex2f(x, y + size);
    }
    glEnd();
}
// ������
static void drawRect(const rectangle& rt, crvec color)
{
    glLineWidth(2);
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_LINE_LOOP);
    glVertex2f(rt.x, rt.y);
    glVertex2f(rt.x + rt.w, rt.y);
    glVertex2f(rt.x + rt.w, rt.y + rt.h);
    glVertex2f(rt.x, rt.y + rt.h);
    glEnd();
}
static void drawRectBorder(const rectangle& rt)
{
    glLineWidth(0.2);
    glColor3f(0.58f, 0.58f, 0.58f);
    glBegin(GL_LINES);
    glVertex2f(rt.x - 2.5, rt.y - 5);
    glVertex2f(rt.x - 2.5, rt.y + rt.h + 2.5);
    glVertex2f(rt.x - 2.5, rt.y + rt.h + 2.5);
    glVertex2f(rt.x + rt.w + 5, rt.y + rt.h + 2.5);
    glVertex2f(rt.x + rt.w + 5, rt.y + rt.h + 2.5);
    glVertex2f(rt.x + rt.w + 5, rt.y - 5);
    glVertex2f(rt.x + rt.w + 5, rt.y - 5);
    glVertex2f(rt.x - 2.5, rt.y - 5);
    glEnd();
}
static void drawRectQuad(const rectangle& rt, crvec color)
{
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2f(rt.x, rt.y);
    glVertex2f(rt.x + rt.w, rt.y);
    glVertex2f(rt.x + rt.w, rt.y + rt.h);
    glVertex2f(rt.x, rt.y + rt.h);
    glEnd();
}

// ����������
void drawSPolygon(const polyline2& polygon) {
    glBegin(GL_LINE_STRIP);

    for (const point2& point : polygon) {
        glVertex3f(
            cos(point.y * M_PI / 180.0f) * cos(point.x * M_PI / 180.0f),
            sin(point.y * M_PI / 180.0f),
            cos(point.y * M_PI / 180.0f) * sin(point.x * M_PI / 180.0f)
        );
    }
    glEnd();
}

// ������ϵ
void drawCoord0(crvec position = vec3::ZERO)
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    {
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glEnd();
    }
    {
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glEnd();
    }
    {
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glEnd();
    }
    glPopMatrix();
}
void drawCoord(crvec position = vec3::ZERO)
{
    // �ر���Ȳ���
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glLineWidth(2);
    {
        // X axis
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.8f, 0.1f, 0.0f);
        glVertex3f(0.8f, -0.1f, 0.0f);
        glEnd();
    }
    {
        // Y axis
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.1f, 0.8f, 0.0f);
        glVertex3f(-0.1f, 0.8f, 0.0f);
        glEnd();
    }
    {
        // Z axis
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.1f, 0.0f, 0.8f);
        glVertex3f(-0.1f, 0.0f, 0.8f);
        glEnd();
    }
    glLineWidth(1);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}

void drawCoord2(crvec position = vec3::ZERO)
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    {
        // X axis
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.9f, 0.1f, 0.0f);
        glVertex3f(0.9f, -0.1f, 0.0f);
        glEnd();

        // Tick marks
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glBegin(GL_LINES);
        for (float i = 0.1f; i < 1.0f; i += 0.1f)
        {
            glVertex3f(i, -0.01f, 0.0f);
            glVertex3f(i, 0.01f, 0.0f);
        }
        glEnd();
    }
    {
        // Y axis
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.1f, 0.9f, 0.0f);
        glVertex3f(-0.1f, 0.9f, 0.0f);
        glEnd();

        // Tick marks
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glBegin(GL_LINES);
        for (float i = 0.1f; i < 1.0f; i += 0.1f)
        {
            glVertex3f(-0.01f, i, 0.0f);
            glVertex3f(0.01f, i, 0.0f);
        }
        glEnd();
    }
    {
        // Z axis
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.1f, 0.0f, 0.9f);
        glVertex3f(-0.1f, 0.0f, 0.9f);
        glEnd();

        // Tick marks
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glBegin(GL_LINES);
        for (float i = 0.1f; i < 1.0f; i += 0.1f)
        {
            glVertex3f(0.0f, -0.01f, i);
            glVertex3f(0.0f, 0.01f, i);
        }
        glEnd();
    }
    glPopMatrix();
}
void drawCoord(crcd3 c)
{
    glPushMatrix();
    glTranslatef(c.o.x, c.o.y, c.o.z);
    {
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f((c.ux).x, (c.ux).y, (c.ux).z);
        glEnd();
    }
    {
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f((c.uy).x, (c.uy).y, (c.uy).z);
        glEnd();
    }
    {
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f((c.uz).x, (c.uz).y, (c.uz).z);
        glEnd();
    }
    glPopMatrix();
}

// ���½�����ϵ
void drawCoord_RightBottom(crvec position = vec3::ZERO)
{
    // �ر���Ȳ���
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glLineWidth(2);
    {
        // X axis
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.8f, 0.1f, 0.0f);
        glVertex3f(0.8f, -0.1f, 0.0f);
        glEnd();
    }
    {
        // Y axis
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.1f, 0.8f, 0.0f);
        glVertex3f(-0.1f, 0.8f, 0.0f);
        glEnd();
    }
    {
        // Z axis
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glEnd();

        // Arrow
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.1f, 0.0f, 0.8f);
        glVertex3f(-0.1f, 0.0f, 0.8f);
        glEnd();
    }
    glLineWidth(1);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
}
// ����������
void drawGroundGrid(float size, float spacing)
{
    // ������ɫ
    glColor4f(0.25f, 0.25f, 0.25f, 1.0f);

    // �����߿�ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glLineWidth(1);

    // ��������
    glBegin(GL_LINES);
    for (float x = -size; x <= size; x += spacing)
    {
        if (x == 0)
        {
            glColor4f(0.5f, 0.75f, 0.75f, 1.0f);
            glLineWidth(2);
        }
        else
        {
            glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
            glLineWidth(0.5);
        }
        glVertex3f(x, 0, -size);
        glVertex3f(x, 0, size);
    }
    for (float z = -size; z <= size; z += spacing)
    {
        if (z == 0)
        {
            glColor4f(0.75f, 0.5f, 0.5f, 1.0f);
            glLineWidth(2);
        }
        else
        {
            glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
            glLineWidth(0.5);
        }
        glVertex3f(-size, 0, z);
        glVertex3f(size, 0, z);
    }
    glEnd();

    // �ر��߿�ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void drawWalls(float size, float floor, float height)
{
    glBegin(GL_QUADS);

    // ǰǽ
    glColor4f(0.115f, 0.025f, 0.0f, 0.9f); // ��͸����ɫ
    glVertex3f(-size, floor, -size);  // ����
    glVertex3f(size + 1, floor, -size);   // ����
    glVertex3f(size + 1, floor + height, -size); // ����
    glVertex3f(-size, floor + height, -size); // ����

    // ��ǽ
    glVertex3f(-size, floor, size + 1);   // ����
    glVertex3f(size + 1, floor, size + 1);    // ����
    glVertex3f(size + 1, floor + height, size + 1); // ����
    glVertex3f(-size, floor + height, size + 1); // ����

    // ��ǽ
    glVertex3f(-size, floor, -size);  // ����
    glVertex3f(-size, floor, size + 1);   // ����
    glVertex3f(-size, floor + height, size + 1); // ����
    glVertex3f(-size, floor + height, -size); // ����

    // ��ǽ
    glVertex3f(size + 1, floor, -size);   // ����
    glVertex3f(size + 1, floor, size + 1);    // ����
    glVertex3f(size + 1, floor + height, size + 1); // ����
    glVertex3f(size + 1, floor + height, -size); // ����

    glEnd();
}
void drawGroundGrid_Filled(float size, float y, float spacing)
{
    // ���û��
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ��������
    glBegin(GL_QUADS);
    for (float x = -size; x <= size; x += spacing)
    {
        for (float z = -size; z <= size; z += spacing)
        {
            if ((int)(x / spacing) % 2 == 0)
            {
                if ((int)(z / spacing) % 2 == 0)
                {
                    glColor4f(0.25f, 0.25f, 0.25f, 0.5f); // ��͸����ɫ
                }
                else
                {
                    glColor4f(0.75f, 0.75f, 0.75f, 0.5f); // ��͸��ǳ��ɫ
                }
            }
            else
            {
                if ((int)(z / spacing) % 2 == 0)
                {
                    glColor4f(0.75f, 0.75f, 0.75f, 0.5f); // ��͸��ǳ��ɫ
                }
                else
                {
                    glColor4f(0.25f, 0.25f, 0.25f, 0.5f); // ��͸����ɫ
                }
            }

            glVertex3f(x, y, z);
            glVertex3f(x + spacing, y, z);
            glVertex3f(x + spacing, y, z + spacing);
            glVertex3f(x, y, z + spacing);
        }
    }
    glEnd();

    // ����ǽ��
    drawWalls(size, 0, -0.1);

    // ���½�����ϵ
    drawCoord_RightBottom();

    // ���û��
    glDisable(GL_BLEND);

}

// ���ռ�����
void drawSpaceGrid(vec3 size, float spacing)
{
    // ������ɫ
    glColor4f(0.25f, 0.25f, 0.25f, 1.0f);

    // �����߿�ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ��������
    glBegin(GL_LINES);
    for (float x = -size.x; x <= size.x; x += spacing)
    {
        for (float y = -size.y; y <= size.y; y += spacing)
        {
            // ������ֱ��
            glVertex3f(x, y, -size.z);
            glVertex3f(x, y, size.z);
        }
        for (float z = -size.z; z <= size.z; z += spacing)
        {
            // ����ˮƽ��
            glVertex3f(x, -size.y, z);
            glVertex3f(x, size.y, z);
        }
    }
    for (float y = -size.y; y <= size.y; y += spacing)
    {
        for (float z = -size.z; z <= size.z; z += spacing)
        {
            // ������ֱ��
            glVertex3f(-size.x, y, z);
            glVertex3f(size.x, y, z);
        }
    }
    glEnd();

    // �ر��߿�ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
// ����
void drawLace(int x, int y, int width, int height, int type)
{
    float r = 0.8; // �������ߵİ뾶
    float angle = 0.0; // �������ߵ���ʼ�Ƕ�
    float step = 0.1; // �������ߵĲ���

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(width, height, 1.0);

    glBegin(GL_LINE_STRIP);

    switch (type) {
    case 0: // ���Ļ���
        for (float i = 0.0; i <= 1.0; i += step) {
            glVertex2f(r * cos(angle), r * sin(angle));
            angle += 2.0 * M_PI * i;
        }
        break;

    case 1: // ���ƻ���
        for (float i = 0.0; i <= 1.0; i += step) {
            glVertex2f(r * cos(angle), r * sin(angle));
            angle += 2.0 * M_PI * i;
            glVertex2f(r * cos(angle), r * sin(angle));
            angle -= 2.0 * M_PI * i;
        }
        break;

        // �������͵Ļ��߻��ƺ������Լ������

    default:
        break;
    }

    glEnd();
    glPopMatrix();
}
// --------------------------------------
// ������ʾ��ά����
// --------------------------------------
void drawVecn(rvecn v) {
    int dimensions = v.val.size(); // ά����

    // �����ӿ�
    int viewportSize = SCREENX / dimensions; // ÿ���ӿڵĴ�С
    for (int i = 0; i < dimensions; i++) {
        glViewport(i * viewportSize, 0, viewportSize, 400 / dimensions);
        glClearColor(0.18f, 0.18f, 0.18f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // ����������
        glColor3f(0.5, 0.5, 0.5);
        glBegin(GL_LINES);
        glVertex3f(-1.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, -1.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        if (dimensions > 2) {
            glVertex3f(0.0, 0.0, -1.0);
            glVertex3f(0.0, 0.0, 1.0);
        }
        glEnd();

        // ���Ƶ�
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_POINTS);
        glVertex3f(v[i], v[(i + 1) % dimensions], 0);
        glEnd();
    }
}
// --------------------------------------
// draw floatmap
// --------------------------------------
void drawFloatmap(crvec o, int step)
{
    glPushMatrix();
    glTranslatef(o.x, o.y, o.z);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    int x = 0;
    glBegin(GL_LINE_STRIP);
    for (auto& v : GTable::floatmap)
    {
        glVertex3f(o.x + x, o.y, o.z);
        glVertex3f(o.x + x, o.y + v.second, o.z);
        x += step;

        std::string value_str = v.first;
        glRasterPos2f(o.x + 0.5f * (10 - value_str.length() * 8), o.y + 0.5f * (30 - 16));
        for (int i = 0; i < value_str.length(); ++i) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, value_str[i]);
        }
    }
    glEnd();
    glPopMatrix();
}
static void draw_edge(const VERLIST& e)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(8.0);

    glBegin(GL_LINE_STRIP);
    for (const vertex& it : e)
    {
        if (it.cor != 0)
        {
            vec3 cor = torgb(it.cor);
            glColor4f(cor.r, cor.g, cor.b, 1.0);
        }
        glVertex2f(it.p.x, it.p.y);
    }
    glEnd();

    glBegin(GL_POINTS);
    glPointSize(15.8f);
    for (const vertex& it : e)
    {
        glVertex2f(it.p.x, it.p.y);
    }
    glEnd();
  
    // �ر��߿�ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// ======================================
// draw hlr
// ======================================
vec2 g_hlr_scale = vec2::ONE;
csys2 g_coord;
MAP<string, std::vector<hlr_data_t>> hlr_map;

// --------------------------------------
static void drawhlr(crstr type)
{
    glPushMatrix();
    real scl = (real)SCREENY / SCREENX * camera.aspect;
    glScalef(g_hlr_scale.x, g_hlr_scale.y * scl, 1.0);
   
    std::vector<hlr_data_t>& hlr = hlr_map[type];
    if(hlr.empty())
        prim_projector::get_hlr_bygroup(hlr, type, false);
    
    for (auto& it : hlr)
    {
        glBegin(GL_LINES);
        glVertex2f(400 + it.p1.x * 180, (400 - it.p1.y * 180));
        glVertex2f(400 + it.p2.x * 180, (400 - it.p2.y * 180));
        glEnd();
    }

#ifdef DEBUG_HLR
    {// ���ƺ�ɫ���ԲȦ
        glColor3f(1.0, 0.0, 0.0);
        for (auto& it : hlr)
        {
            glBegin(GL_LINE_LOOP);
            real r = c_tor_dis_level0 * 180;
            for (int i = 0; i <= 8; i++)
            {
                float angle = i * 2 * PI / 8;
                glVertex2f(400 + it.p1.x * 180 + r * cos(angle), (400 - it.p1.y * 180) + r * sin(angle));
            }
            glEnd();
        }
    }

    if(!hlr.empty())
    {
        {// ��ʼ��
            glPointSize(12.0f);
            glBegin(GL_POINTS);
        
            glVertex2f(400 + hlr.front().p1.x * 180, (400 - hlr.front().p1.y * 180));
            glVertex2f(400 + hlr.back().p2.x * 180, (400 - hlr.back().p2.y * 180));
        }
        glEnd();

        {// �����߶˵�
            glPointSize(4.0f);
            glBegin(GL_POINTS);
            for (auto& it : hlr)
            {
                //glVertex2f(400 + it.p1.x * 180, (400 - it.p1.y * 180));
                glVertex2f(400 + it.p2.x * 180, (400 - it.p2.y * 180));
            }
            glEnd();
        }
    }
#endif
    glPopMatrix();
}
// --------------------------------------
static void drawhlr()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1);

    glPointSize(2);
    glColor4f(1.0f, 1.0f, rrnd(0.8f, 1.0f), 1.0f);
    drawhlr("border_view");

    glPointSize(1);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    if(show_hidden_hlr)
        drawhlr("border_img_view");

    glPointSize(2);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    drawhlr("center_view");
    
    // �ر��߿�ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void lineLoop2(crstr view = "XY") 
{
    glPushMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glScalef(280, 280, 280);

    std::vector<VERLIST>* pestack;
    getStack((void**)&pestack);

    for (int i = 0; i < pestack->size(); i++) 
    {
        glBegin(GL_LINE_LOOP);
        for (auto& it : (*pestack)[i])
        {
            float X = it.p.x;
            float Y = it.p.y;
            float Z = it.p.z;

            if (view == "XY") {
                glVertex3f(X, Y, Z);
            }
            else if (view == "XZ") {
                glVertex3f(X, Z, Y);
            }
            else if (view == "YZ") {
                glVertex3f(Y, Z, X);
            }
        }
        glEnd();
    }
    glPopMatrix();
}

// ======================================
// draw equeque
// ======================================
bool b_draw_estack = true;
// draw equeque
static void draw_estack(void)
{
    std::vector<VERLIST>* pestack;

    if(!getStack((void**)&pestack))
        return;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_POINT_SMOOTH);
    
    for (int i = 0; i < pestack->size(); i++)
    {  
        glColor4f(hash0(i + 1.0), hash0(i * i + 2.0), hash0(i * i * i + 1.8), 1.0f);
        glBegin(GL_LINE_STRIP);
        glPointSize(2.0);
        for (const vertex& it : (*pestack)[i])
        {
            if (it.cor != 0)
            {
                vec3 cor = torgb(it.cor);
                glColor4f(cor.r,cor.g,cor.b,1.0);
            }
            glVertex3f(it.p.x, it.p.y, it.p.z);
        }
        glEnd();

        //{// ���Ƶ�Ч��
        //    glBegin(GL_POINTS);
        //    glPointSize(0.1);
        //    if ((*pestack)[i].size() == 2)
        //    {
        //        glVertex3f((*pestack)[i][0].p.x, (*pestack)[i][0].p.y, (*pestack)[i][0].p.z);
        //    }
        //    else
        //    {
        //        for (const vertex& it : (*pestack)[i])
        //        {
        //            glVertex3f(it.p.x, it.p.y, it.p.z);
        //        }
        //    }
        //    glEnd();
        //}
    }
    // �ر��߿�ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// �������������ݿ��ȡ�����߲���ʾ
static void draw_estack2(void)
{
    glEnable(GL_POINT_SMOOTH);
    
    std::vector<VERLIST>* pestack;

    if (!getStack((void**)&pestack))
        return;

    for (int i = 0; i < pestack->size(); i++)
    {
        auto& poly = (*pestack)[i];
        if (poly.empty())
            continue;

        int type = poly.front().tag;
        if (type == 1)
        {
            glLineWidth(1.0f);
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else  if (type == 2)
        {
            glLineWidth(1.0);
            glColor3f(0.0f, 0.0f, 1.0f);
        }
        else
        {
            glLineWidth(1.0);
            glColor3f(0.0f, 1.0f, 0.0f);
        }

        glBegin(GL_LINES);
        for (auto& it : poly)
        {
            glVertex2f(it.p.x, it.p.y);
            if (it == poly.front() || it == poly.back())
                continue;
            glVertex2f(it.p.x, it.p.y);
        }
        glEnd();

        /*glPointSize(2.0f);
        glBegin(GL_POINTS);
        for (auto& it : poly)
        {
            glVertex2f(it.p.x, it.p.y);
        }
        glEnd();*/
    }
}