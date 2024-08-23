#pragma once
// 绘制接口
namespace IMAGE 
{
    extern void addImage(const std::string& filename, const rectangle& rect);
    extern void displayImages();
}

namespace shape3d {
    extern void drawCircle(const crd3& c, real r);
    extern void drawCylinder(float radius, float height);
    extern void drawSphere(float radius, int slices, int stacks);
    extern void drawCone(float radius, float height, int slices);
    extern void drawTorus(float majorRadius, float minorRadius, int majorSlices, int minorSlices);
    extern void drawSector(float radius, float angle, int slices);
}

namespace shape2d {
    extern void drawCircle(float x, float y, float radius, int triangleAmount);
    extern void drawSquare(float x, float y, float width, float height);
    extern void drawRect(float x, float y, float width, float height);
    extern void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
    extern void drawRay(float x, float y, float length);
    extern void drawArrow(float x, float y, float length, float arrowSize);
    extern void drawEllipse(float x, float y, float radiusX, float radiusY, int triangleAmount);
    extern void drawParabola(float x, float y, float a, float b, float c, float halfWidth, float stepSize);
    extern void drawAngle(float x, float y, float angle, float length);
    extern void drawSpiral(float x, float y, float radius, float numLoops, float stepSize);
    extern void drawPolyline(const std::vector<vec2>& points);
    extern void drawLine(float x1, float y1, float x2, float y2);
    extern void drawCurve(const std::vector<vec2>& points);
}
namespace CMD {
    extern void onmsg(const std::string& msg, const std::string& body);
}

// 给DLL调用
typedef void (*Msgfunc)(const std::string& msg, const std::string& body, std::string& out);
typedef void(*drawCircle)(float x, float y, float radius, int triangleAmount);

typedef void (*InitFunc)(
    Msgfunc,
    drawCircle
    );
typedef void (*DrawFunc)(float);
typedef void (*ExitFunc)();