// ******************************************************
// ���� diagrams
// ******************************************************
namespace diagrams
{
    /* ���庯�� */
    double func(double x) {
        return sin(x * 8) * 0.25 + 0.5;
    }

    /* ���������� */
    void drawAxis() {
        glLineWidth(4.0);
        glColor3f(1.0, 1.0, 1.0); // ������ɫΪ��ɫ
        glBegin(GL_LINES); // ��ʼ�����߶�
        glVertex2f(-1.0, 0.0); // �����������
        glVertex2f(1.0, 0.0); // �����յ�����
        glVertex2f(0.0, -1.0);
        glVertex2f(0.0, 1.0);
        glEnd(); // ���������߶�
    }

    /* ���ƺ������� */
    void drawFunction() {
        glLineWidth(3.0);
        glColor3f(0.0, 1.0, 0.0); // ������ɫΪ��ɫ
        glBegin(GL_LINE_STRIP); // ��ʼ�����߶�
        for (double x = -1.0; x <= 1.0; x += 0.01) {
            glVertex2f(x, func(x)); // ���õ�����
        }
        glEnd(); // ���������߶�
    }

    /* ���Ƹ�� */
    void drawGrid() {
        glLineWidth(1.0);
        glColor3f(0.5, 0.5, 0.5); // ������ɫΪ��ɫ
        glBegin(GL_LINES); // ��ʼ�����߶�
        for (double x = -1.0; x <= 1.0; x += 0.1) {
            glVertex2f(x, -1.0); // �����������
            glVertex2f(x, 1.0); // �����յ�����
        }
        for (double y = -1.0; y <= 1.0; y += 0.1) {
            glVertex2f(-1.0, y);
            glVertex2f(1.0, y);
        }
        glEnd(); // ���������߶�
    }

    /* ��ʾ�ص����� */
    void display() {
        drawAxis(); // ����������
        drawFunction(); // ���ƺ�������
        drawGrid(); // ���Ƹ��
    }
}