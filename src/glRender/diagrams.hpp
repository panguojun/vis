// ******************************************************
// 绘制 diagrams
// ******************************************************
namespace diagrams
{
    /* 定义函数 */
    double func(double x) {
        return sin(x * 8) * 0.25 + 0.5;
    }

    /* 绘制坐标轴 */
    void drawAxis() {
        glLineWidth(4.0);
        glColor3f(1.0, 1.0, 1.0); // 设置颜色为白色
        glBegin(GL_LINES); // 开始绘制线段
        glVertex2f(-1.0, 0.0); // 设置起点坐标
        glVertex2f(1.0, 0.0); // 设置终点坐标
        glVertex2f(0.0, -1.0);
        glVertex2f(0.0, 1.0);
        glEnd(); // 结束绘制线段
    }

    /* 绘制函数曲线 */
    void drawFunction() {
        glLineWidth(3.0);
        glColor3f(0.0, 1.0, 0.0); // 设置颜色为绿色
        glBegin(GL_LINE_STRIP); // 开始绘制线段
        for (double x = -1.0; x <= 1.0; x += 0.01) {
            glVertex2f(x, func(x)); // 设置点坐标
        }
        glEnd(); // 结束绘制线段
    }

    /* 绘制格点 */
    void drawGrid() {
        glLineWidth(1.0);
        glColor3f(0.5, 0.5, 0.5); // 设置颜色为白色
        glBegin(GL_LINES); // 开始绘制线段
        for (double x = -1.0; x <= 1.0; x += 0.1) {
            glVertex2f(x, -1.0); // 设置起点坐标
            glVertex2f(x, 1.0); // 设置终点坐标
        }
        for (double y = -1.0; y <= 1.0; y += 0.1) {
            glVertex2f(-1.0, y);
            glVertex2f(1.0, y);
        }
        glEnd(); // 结束绘制线段
    }

    /* 显示回调函数 */
    void display() {
        drawAxis(); // 绘制坐标轴
        drawFunction(); // 绘制函数曲线
        drawGrid(); // 绘制格点
    }
}