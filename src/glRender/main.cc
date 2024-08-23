/**                   【基于OpenGL的通用可视化平台】
* 
*                          最方便的方式进行
*                          最自由的可视化
* 
*/
#define WINDOWS

#include <stdio.h>
#include <stdlib.h> 
#include <stdarg.h>
#include <math.h>
#include <tgmath.h>
#include <algorithm>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream >
#include <set>
#include <regex>
#include <stdexcept>
#include <cmath>
#include <array>
#include <deque>
#include <map>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <time.h>
#include <mutex>

#include "GL/glew.h"
#include "GL/glut.h"
#include "GL/freeglut.h"
#include "GL/GLFW/glfw3.h"

#include <Windows.h>
#include <shellapi.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <omp.h>

// ***************************************************************
// 特征定义
// ***************************************************************
#define     BECHO               true;           // 打印信息开关
#define		IMAGESCALE		    8			    // 画布尺寸
#define		real			    float			// 精度类型
#define		EPSINON			    1e-3			// 精度
#define		MAXZ			    100.0
#define		EXPORT_SCALE	    1.0
#define		USIZE0			    0.01f
#define		USIZE			    (USIZE0 / EXPORT_SCALE)
#define     M_PI                PI
#define     twicePi             (2 * PI)
#define     twoPi               (2 * PI)

#define     NORMV(p1, p2, p3)	(p2 - p1).cross(p3 - p1)		// 法线方向
#define     NORM(p1, p2, p3)	NORMV(p1, p2, p3).normcopy() 

#define		DEVICE_CALLABLE
#define     VERTEX_COLOR
//#define   MIRROR_Z           // Z方向镜像

#pragma warning(disable:4244)
#pragma warning(disable:4305)
#pragma warning(disable:4267)
#pragma warning(disable:4819)
#pragma warning(disable:4018)

// LIBS
#pragma comment(lib, "GCU.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "freeglut.lib")

// -------------------------------------------------
// 通用头文件
// -------------------------------------------------
#include    "com.hpp"
using namespace std;

// -------------------------------------------------
// 全局变量
// -------------------------------------------------
// 缩放
#define GLOBAL_SCALE_DEF
real GLOBAL_SCALE = 1.0f;

// 网络IP
std::string     CLIENT  =   "127.0.0.1";
std::string     HOSTIP  =   "127.0.0.1";

// 舞台类型
std::string stage       =   "ui"; // "login";
std::string substage    =   "";

// 屏幕参数
int	        SCREENX     =   1288;
int		    SCREENY     =   800;

// offset
std::vector<std::pair<std::string, int>> offsetX = { {stage, 0} };
std::vector<std::pair<std::string, int>> offsetY = { {stage, 0} };

#define OFFSET_X        offsetX.back().second
#define OFFSET_Y        offsetY.back().second

#define OFFSET_PUSH     offsetX.push_back({stage, 0}); offsetY.push_back({stage, 0});
#define OFFSET_POP      if(offsetX.size() >= 2){offsetX.pop_back(); offsetY.pop_back();}

bool b_fullscreen = false;
int mouse_x = 0;                                // 鼠标x坐标
int mouse_y = 0;                                // 鼠标y坐标
bool mouse_left_down = false;                   // 鼠标左键是否按下
bool mouse_middle_down = false;                 // 鼠标中键是否按下

string imgui_main_lua;
string imgui_anim_lua;

bool g_echo_switch = true;
bool grid_flag = false;
bool show_hidden_hlr = true;

int framecount = 0;
bool banimation = 0;

// --------------------------------------------------------------
extern      EXPORT_API    HANDLE	hConsole;	// 标准输出句柄
extern "C"
{
    int     doPHG(const char* script);
    bool    getStack(void** _estack);
    int     getVcnt();
    bool    getVdata(vec3* v, int vcnt);
    bool    getNorm(vec3* n, int tcnt, int vcnt);
    int     getTcnt();
    bool    getTdata(int* t, int tcnt);
    bool    getCdata(int* c, int vcnt);
}
namespace CMD{ extern void parse_command(const std::string& command);}
extern void front_view();                       // stream and draw

extern const char* RunString(const char* szLua);
extern void LoadImguiBindings();
// --------------------------------------------------------------

void mouse_viewport(int x_mouse, int y_mouse, int& x_vp, int& y_vp, int w, int h, int window_width, int window_height) {
    x_vp = x_mouse * w / window_width;
    y_vp = (window_height - y_mouse) * h / window_height;
}

std::string GetExeDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}
std::string GetLUAFilePath(const std::string& fileName)
{
    std::string exeDirectory = GetExeDirectory();
    if (exeDirectory.find("x64\\Release") != string::npos)
        return fileName;
    return exeDirectory + "\\" + fileName;
}

// --------------------------------------------------------------
#include    ".sys/str.hpp"
#include    "../.thirdpart/pngDec.hpp"

// core
#include    "math/functor.hpp"
#include    "geom/geom.hpp"
#include    "geom/geom2.hpp"
#include    "geom/geom_math.hpp"
#include    "geom/geom_math2.hpp"
#include    "boundary/linspace.hpp"
#include    "boundary/bound_clamp.hpp"
#include    "boundary/boundary.hpp"
#include    "boundary/bound_val.hpp"
#include    "boundary/rectangle.hpp"
#include    "md_data/equeue.hpp"
#include    "geom/curve.hpp"
#include    "poly/poly_curve.hpp"
#include    "geom/curve_math.hpp"
#include    "geom/curve_differ.hpp"
#include    "poly/segment.hpp" 

// 向量/坐标系数学扩展
#include    "vec_alg/vec_math_ex.hpp"
#include    "vec_alg/projection.hpp"

#include    "vec_alg/blendex.hpp"
#include    "vec_alg/blend2ex.hpp"

// submesh
#include    "md_data/submesh.hpp"
#include    "md_data/smqueque.hpp"

// pm
#include    "pm/pm_api.hpp"
#include    "pm/pm_wireframe.hpp"               // wireframe
#include    "pm/pm_api_mt.hpp"

#include    "pm/etart.hpp"                      // 直接绘图API

#include    "pm/pm_prim.hpp"
#include    "pm/pm_pipeline.hpp"
#include    "pm/pm_prim2.hpp"

// curve
#include    "geom/curve2_crd.hpp"

// range
#include    "boundary/range2.hpp"
#include    "boundary/range.hpp"

// primitive
#include    "md_data/primitive.hpp"

// hlr
#include    "../SceneHLR/hlr/hlr_head.hpp"

// gdb
#include    "../external/sqlite3/sqlite3.h"
#include    "../SceneHLR/scene/scene_head.hpp" // scene
#include    "gdb.hpp"

// http
#include    "http_server.hpp"

#include    "iGraphics.hpp"

#include    "vec_alg/matrix_lit.hpp"
#include    "boundary/topo_tree.hpp"

#include    "topoE_mesh.hpp"

// poly
#include    "poly/poly_coord_math.hpp"
#include    "poly/poly_math.hpp"
#include    "poly/polyline.hpp"
#include    "poly/triangle.hpp"
#include    "poly/polygon.hpp"
#include    "poly/poly_holes.hpp"

// GTable
#include    "md_data/data_table.hpp"

// spin
#include    "spin.hpp"

// http
#include    ".thirdpart/httplib.hpp"

// json
#include    ".thirdpart/nlohmannjson.hpp"

#include    "gl_mesh.hpp"

// file
#include    ".thirdpart/json.hpp"

#include    "IO/sm_io.hpp"

#include    "camera.hpp"
#include    "gl_sym.hpp"
#include    "visGL.hpp"
#include    "font_render.hpp"

#include    "shape2d.hpp"
#include    "strVStream2d.hpp"
#include    "shape3d.hpp"
#include    "strVStream3d.hpp"

// ui
#include    "feature.hpp"

#include    "cmd.hpp"

// Lua
extern "C" {
#include "../external/lua/lua53.h"
}

// -------------------------------------------------
// IMGUI
// -------------------------------------------------
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_internal.h"
#include "../external/imgui/imgui_impl_opengl3.h"

#include "draw.hpp"
#include "../external/imgui/lua_bindings/imgui_lua_bindings.hpp"

#include    "ui.hpp"

// -------------------------------------------------
#include "featrue_vis.hpp"
#include "grid_vis.hpp"
#include "topo_vis.hpp"

#include "shader.hpp"

// -------------------------------------------------
// screen quad
// -------------------------------------------------
GLuint blurShader;
GLuint quadVAO, quadVBO;
GLuint screenTexture;

void setupTexture(int width, int height)
{
    // 生成纹理ID
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 分配纹理内存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // 生成Mipmap（如果需要）
    glGenerateMipmap(GL_TEXTURE_2D);

    // 解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);
}
void setupQuad(int windowWidth, int windowHeight)
{
    // 加载模糊Shader
    blurShader = load_shader("shader/vertex_shader.glsl", "shader/fragment_shader.glsl");

    float quadVertices[] = {
        // Positions     // TexCoords
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 创建纹理
    setupTexture(windowWidth, windowHeight);
}
void renderQuad()
{
    // 激活模糊着色器
    glUseProgram(blurShader);

    // 绑定输入纹理
    glActiveTexture(GL_TEXTURE0); // 激活纹理单元 0
    glBindTexture(GL_TEXTURE_2D, screenTexture); // 绑定纹理
    glUniform1i(glGetUniformLocation(blurShader, "texture1"), 0); // 设置着色器中的纹理单元

    // 绑定VAO
    glBindVertexArray(quadVAO);

    // 绘制四边形
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 解绑VAO
    glBindVertexArray(0);

    // 解绑纹理
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);  // 如果之前启用了混合
    glDisable(GL_DEPTH_TEST); // 如果之前启用了深度测试
}

// -------------------------------------------------
// INIT
// -------------------------------------------------
void init() 
{
    glClearColor(0.158f, 0.158f, 0.158f, 1.0f);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
   
    GLfloat light_direction[] = { 100.0f, 100.0f, 100.0f, 0.0f }; // 光源方向向量
    glLightfv(GL_LIGHT0, GL_POSITION, light_direction); // 设置光源方向

    GLfloat ambient_light[] = { 0.25f, 0.125f, 0.0f, 1.0f }; // 环境光颜色
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light); // 设置环境光颜色

    GLfloat mat_diffuse[] = { 1, 1, 1, 1.0f }; // 材质颜色
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); // 设置材质颜色

   // GLfloat mat_specular[] = { 0.25f, 0.25f, 0.25f, 1.0f }; // 材质颜色
   // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular); // 设置镜面反射颜色

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); // 开启颜色材质
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // 设置颜色材质为环境光和漫反射色

    // 开启抗锯齿
    glutSetOption(GLUT_MULTISAMPLE, 2);
    glEnable(GL_MULTISAMPLE);
   // glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

   // glEnable(GL_CULL_FACE);

   // IMAGE::test();

    //dllLib::load();

    STR::readfile(imgui_main_lua, GetLUAFilePath("imgui/main.lua"));
    //STR::readfile(imgui_anim_lua, GetLUAFilePath("imgui/anim.lua"));

    // 初始状态设置
    GTable::intmap["progress"] = 1;
    GTable::intmap["draw hlr"] = 8;

   // GTable::markmap.EPUSH(789, crd3({ 0,1,0 }), vec3::UY);

    // setupQuad(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
// -------------------------------------------------
// DRAW
// -------------------------------------------------
void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    glScalef(1, 1, 1);

    glutSetOption(GLUT_MULTISAMPLE, 2);
    glEnable(GL_MULTISAMPLE);

    if ("3d" == stage) 
    {
        if(banimation)
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        else
            glClearColor(0.158f, 0.158f, 0.158f, 1.0f);

        glEnable(GL_MULTISAMPLE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND); // 开启混合模式
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置混合因子

        if (grid_flag)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        camera.perspective();
        camera.lookAt(camera.eye);

        glPushMatrix();
        glScalef(GLOBAL_SCALE, GLOBAL_SCALE, GLOBAL_SCALE);
       
        PM::draw();

        glPopMatrix();

        glDisable(GL_BLEND); // 关闭混合模式
        glDisable(GL_LIGHTING);
        
        auto it = GTable::intmap.find("showground");
        if (it == GTable::intmap.end() || it->second)
        {
           // glLineWidth(2);
           // drawGroundGrid(20.0f, 5.0f);
            glLineWidth(1);
            drawGroundGrid_Filled(20.0f,0, 1.0f);
        }

        glPushMatrix();
        glScalef(GLOBAL_SCALE, GLOBAL_SCALE, GLOBAL_SCALE);
        for(const auto [a, b, c] : GTable::markmap)
        {
            glColor4f(1.25f, 1.25f, 1.25f, 1.0f);
            draw_dim_num(a, b, (OffsetType)c);
        }

        glPopMatrix();

        {
            glPushMatrix();
                glScalef(GLOBAL_SCALE, GLOBAL_SCALE, GLOBAL_SCALE);

                if (b_draw_estack)
                    draw_estack();

            glPopMatrix();

            glPushMatrix();
                auto it = GTable::intmap.find("showground");
                if (it == GTable::intmap.end() || it->second)
                {
                    drawCoord();

                    // light
                    drawPoint3_cs(vec3(10.0f, 10.0f, 10.0f), 0.18, vec3(1.0f, 1.0f, 1.0f));
                }
            glPopMatrix();
        }
    }

    // 2D 
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREENX, 0, SCREENY, -1, 1); // 设置投影矩阵为正交投影
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // 设置视图矩阵和模型矩阵为单位矩阵

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if ("2d" == stage)
    {
        glViewport(18, 18, glutGet(GLUT_WINDOW_WIDTH) - 36, glutGet(GLUT_WINDOW_HEIGHT) - 36);
        glClearColor(0.058f, 0.058f, 0.058f, 1.0f);
        glDisable(GL_MULTISAMPLE);

        strVSteam2d::docmd();
    }

    // 渲染全屏模糊效果
    //renderQuad();
}
void display_ui()
{
    display();

    if ("login" == stage)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetWindowSize(ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
        ImGui::SetNextWindowPos(ImVec2(glutGet(GLUT_WINDOW_WIDTH) / 2 - 158 / 2, 118));
        ImGui::SetNextWindowSize(ImVec2(250, 512));
        ImGui::SetNextWindowBgAlpha(0.0f); // 设置背景窗口透明度为0
        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); // 去掉背景和标题栏
        {
            ImGui::Text("Welcome to Pythagoras' world!", ImVec2(180, 58));

            ImGui::Text(("PHG to " + HOSTIP + ":5088").c_str(), ImVec2(180, 100));

            if (ImGui::Button("( | )", ImVec2(158, 158)))
            {
                stage = "ui";
                glutReshapeWindow(SCREENX, SCREENY);
                glutPostRedisplay();
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    else if ("grid" == stage)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetWindowSize(ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
        ImGui::SetNextWindowBgAlpha(0.0f); // 设置背景窗口透明度为0
        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); // 去掉背景和标题栏

        if (ImGui::Button("<-", ImVec2(60, 40)))
        {
            stage = "ui";
            glutPostRedisplay();
        }

        grid_canvas_scene::display();

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    else if ("topo" == stage)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetWindowSize(ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
        ImGui::SetNextWindowBgAlpha(0.0f); // 设置背景窗口透明度为0
        ImGui::Begin("", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); // 去掉背景和标题栏

        if (ImGui::Button("<-", ImVec2(60, 40)))
        {
            stage = "ui";
            glutPostRedisplay();
        }

        topo_view::draw();
       

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    else if ("ui" == stage || "3d" == stage || "2d" == stage)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGui::SetWindowSize(ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
        ImGui::SetNextWindowPos(ImVec2(glutGet(GLUT_WINDOW_WIDTH) - 100, 24));
        ImGui::SetNextWindowSize(ImVec2(250, 512));
        ImGui::SetNextWindowBgAlpha(0.0f); // 设置背景窗口透明度为0
        ImGui::Begin("vis", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); // 去掉背景和标题栏

        if (("3d" == stage || "2d" == stage) && !(banimation))
        {
            if ("3d" != stage)
            {
                if (ImGui::Button("3d", ImVec2(60, 40)))
                {
                    GTable::intmap["draw hlr"] = 8;
                    b_draw_estack = 1;
                    stage = "3d";
                    if(PM::smesh_list.empty())
                        refresh_sm();
                    
                    glClearColor(0.158f, 0.158f, 0.158f, 1.0f);
                    glutPostRedisplay();
                }
            }
            else
            {
                if (ImGui::Button("hlr2d", ImVec2(60, 40)))
                {
                    stage = "2d";
                    substage = "hlr";

                    glutPostRedisplay();
                }
            }
            if (ImGui::Button("ui", ImVec2(60, 40)))
            {
                stage = "ui";
                glutPostRedisplay();
            }
            
            if (ImGui::Button("dummy", ImVec2(60, 40)))
            {
                show_hidden_hlr = !show_hidden_hlr;

                auto it = GTable::intmap.find("showground");
                if (it == GTable::intmap.end() || it->second)
                {
                    GTable::intmap["showground"] = false;
                }
                else
                    GTable::intmap["showground"] = true;

                glutPostRedisplay();
            }
            if (ImGui::Button("feature", ImVec2(60, 40)))
            {
                stage = "2d";
                substage = "front";

                glutPostRedisplay();
            }
            if (ImGui::Button("grid", ImVec2(60, 40)))
            {
                stage = "grid";
                substage = "";

                glutPostRedisplay();
            }
            if (ImGui::Button("topo", ImVec2(60, 40)))
            {
                stage = "topo";
                substage = "";

                glutPostRedisplay();
            }
            if (ImGui::Button("X", ImVec2(60, 40)))
            {
                exit(0);
            }
        }
        if ("3d" == stage && "anim" == substage)
        {
            if (!imgui_anim_lua.empty())
            {
                RunString(imgui_anim_lua.c_str());
            }
        }

        if ("ui" == stage)
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
            ImGui::SetNextWindowBgAlpha(0.0f); // 设置背景窗口透明度为0
            ImGui::Begin("canvas", nullptr,
                ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); // 去掉背景和标题栏
            //ImGui::Image(bk_image, ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));

            if (!imgui_main_lua.empty())
            {
                RunString(imgui_main_lua.c_str());
            }
            else
            {
                ImGui::Text("I am the Visual Man!");
            }
            if (!lua_net_string.empty())
            {
                RunString(lua_net_string.c_str());
            }
            ImGui::End();
        }
        else if("2d" == stage)
        {
            draw_hlr();
        }

        ImGui::End();

        // 消息提示
        notification::RenderLoop();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    
    glutSwapBuffers();
}

// -------------------------------------------------
// UPDATE
// -------------------------------------------------
void update(int value) 
{
    ImGuiIO& io = ImGui::GetIO();

    io.DeltaTime = 1.0f / 18.0f;

    if (!msgbuffer.empty())
    {
        std::lock_guard<std::mutex> lock(msgbuffer_mutex); // 加锁
        for (auto& it : msgbuffer)
            CMD::onmsg(it.first, it.second);

        msgbuffer.clear();
    }

    if("anim" == substage)
    {
        spin::frame++;              // 自旋

        glutPostRedisplay();        // 通知OpenGL重新绘制窗口
    }

    glutTimerFunc(18, update, 0);   // 16毫秒（约60帧每秒）
}

// -------------------------------------------------
// MOUSE
// -------------------------------------------------
void mouse(int button, int state, int x, int y) 
{
    if ("login" == stage || "topo" == stage || "grid" == stage || "ui" == stage || "2d" == stage || "3d" == stage)
    {
        // 添加imgui的鼠标操作
        ImGuiIO& io = ImGui::GetIO();
        ImGuiContext& g = *GImGui;
        if (g.ActiveIdWindow && ImGui::IsItemClicked())
            return;

        io.MousePos.x = (float)x;
        io.MousePos.y = (float)y;

        io.MouseDown[0] = button == GLUT_LEFT_BUTTON && state == GLUT_DOWN;
        io.MouseDown[1] = button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN;
        io.MouseDown[2] = button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN;
    }
     if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouse_x = x;
            mouse_y = y;
            mouse_left_down = true;
        }
        else if (state == GLUT_UP) {
            mouse_left_down = false;
        }
        glutPostRedisplay();
    }
    if (button == GLUT_MIDDLE_BUTTON) {
        if ("3d" == stage)
        {
            camera.speed = 0.005; // 中键按下稍微移动一点
            if (state > 0) {
                camera.moveBackward();
            }
            else {
                camera.moveForward();
            }
        }
        if (state == GLUT_DOWN) {
            mouse_x = x;
            mouse_y = y;
            mouse_middle_down = true;
        }
        else if (state == GLUT_UP) {
            mouse_middle_down = false;
        }
        glutPostRedisplay();
    }
    if (button == 3 || button == 4) {   // 滚轮向上滚动为3，向下滚动为4
        if ("3d" == stage)
        {
            if (button == 3) {
                camera.moveForward();   // 滚轮向上滚动，摄像机向前移动
            }
            else {
                camera.moveBackward();  // 滚轮向下滚动，摄像机向后移动
            }
            glutPostRedisplay();
        }
        else if ("2d" == stage)
        {
            if (button == 3)
            {
                g_hlr_scale *= 1.1f;
                poly_scaler *= 1.1f;

                OFFSET_X = (OFFSET_X - x) * 1.1f + x;
                OFFSET_Y = (OFFSET_Y - (glutGet(GLUT_WINDOW_HEIGHT) - y)) * 1.1f + (glutGet(GLUT_WINDOW_HEIGHT) - y);
            }
            else
            {
                g_hlr_scale /= 1.1f;
                poly_scaler /= 1.1f;

                OFFSET_X = (OFFSET_X - x) / 1.1f + x;
                OFFSET_Y = (OFFSET_Y - (glutGet(GLUT_WINDOW_HEIGHT) - y)) / 1.1f + (glutGet(GLUT_WINDOW_HEIGHT) - y);
            }
            glutPostRedisplay();
        }
    }
      
    mouse_viewport(x, y, x, y, 
        SCREENX, SCREENY, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    g_coord.o = { (real)OFFSET_X, (real)OFFSET_Y };
    g_coord.s = g_hlr_scale;

    if (button == GLUT_LEFT_BUTTON && glutGetModifiers() == GLUT_ACTIVE_ALT 
        && state == GLUT_DOWN)
    {// ray pick

        camera.perspective();
        camera.lookAt(camera.eye);

        // 获取模型视图矩阵、投影矩阵和视口
        GLdouble modelview[16];
        GLdouble projection[16];
        GLint viewport[4];

        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

        //// 将裁剪空间坐标转换为世界坐标
        GLdouble worldX0, worldY0, worldZ0;
        gluUnProject((GLdouble)mouse_x, (GLdouble)(viewport[3] - mouse_y), 0.0, modelview, projection, viewport, &worldX0, &worldY0, &worldZ0);
        GLdouble worldX1, worldY1, worldZ1;
        gluUnProject((GLdouble)mouse_x, (GLdouble)(viewport[3] - mouse_y), 1.0, modelview, projection, viewport, &worldX1, &worldY1, &worldZ1);

        vec3 dir;

        dir.x = worldX1 - worldX0;
        dir.y = worldY1 - worldY0;
        dir.z = worldZ1 - worldZ0;

        dir.norm();

        //equeue.push({ camera.eye, camera.eye + (dir * 58) });

        pick_ray({ camera.eye, camera.eye + (dir * 58)});

        if (!smqueue.empty() || !smkeyframes.empty())
        {
            if (!smqueue.empty())
                smqueue.move_current();

            PM::setup_mesh_data();

            glutPostRedisplay();
        }
    }
}

// -------------------------------------------------
// 鼠标移动函数
// -------------------------------------------------
void motion(int x, int y)
{
    if ("ui" == stage || "2d" == stage || "3d" == stage)
    {
        // 添加imgui的鼠标操作
        ImGuiIO& io = ImGui::GetIO();
        ImGuiContext& g = *GImGui;
        if (g.ActiveIdWindow && ImGui::IsItemClicked())
            return;

        io.MousePos.x = (float)x;
        io.MousePos.y = (float)y;
    }
    if ("3d" == stage && mouse_left_down)
    {
        int dx1 = x - mouse_x;
        int dy = y - mouse_y;

        camera.rotateLeft(dx1 * 0.01f);
        camera.rotateUp(dy * 0.0025f);
        mouse_x = x;
        mouse_y = y;

        glutPostRedisplay();
    }
    else if ("3d" == stage && mouse_middle_down)
    {
        int dx2 = x - mouse_x;
        int dy = y - mouse_y;
        real coff = (camera.eye - camera.center).len() * 0.0058;
        camera.moveLeft(dx2 * coff);
        camera.moveUp(dy * coff);
        mouse_x = x;
        mouse_y = y;

        glutPostRedisplay();
    }
    else if ("2d" == stage && mouse_left_down)
    {
        int dx = x - mouse_x;
        int dy = y - mouse_y;
        OFFSET_X = (OFFSET_X + dx);
        OFFSET_Y = (OFFSET_Y - dy);
        glViewport(OFFSET_X, OFFSET_Y, SCREENX, SCREENY);
        gluOrtho2D(OFFSET_X, OFFSET_X + SCREENX, OFFSET_Y, OFFSET_Y + SCREENY);
        mouse_x = x;
        mouse_y = y;

        glutPostRedisplay();
    }
}

// -------------------------------------------------
// KEY
// -------------------------------------------------
void keyboard(unsigned char key, int x, int y) {
    // 添加imgui的键盘操作
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[key] = true;

    switch (key) {
    case 'p':
    case 'P':
    {
        PRINTV3(camera.center);
    }break;
    case 'w':
    case 'W':
        camera.speed /= 20.0f;
        camera.moveForward(true);
        glutPostRedisplay();
        break;
    case 's':
    case 'S':
        camera.speed /= 20.0f;
        camera.moveBackward(true);
        glutPostRedisplay();
        break;
    case 'a':
    case 'A':
        camera.speed /= 50.0f;
        camera.moveLeft(1, false);
        glutPostRedisplay();
        break;
    case 'd':
    case 'D':
        camera.speed /= 50.0f;
        camera.moveRight(1, false);
        glutPostRedisplay();
        break;
    case 'q':
    case 'Q':
        camera.moveUp(0.1, false);
        glutPostRedisplay();
        break;
    case 'e':
    case 'E':
        camera.moveDown(0.1, false);
        glutPostRedisplay();
        break;
    case 'I':
    case 'i':
        OFFSET_PUSH;
        stage == "2d" ? stage = "3d" : stage = "2d";
        glutPostRedisplay();
        break;
    case 27:
        if (b_fullscreen) {
            glutReshapeWindow(SCREENX, SCREENY); // 还原窗口大小
            b_fullscreen = false;
            break;
        }
        if (stage == "2d") {
            strVSteam2d::commands.clear();
            OFFSET_POP;
            stage = "ui";
            glClearColor(0.18f, 0.18f, 0.18f, 1.0f);
        }
        else if (stage == "3d") {
            strVSteam3d::commands.clear();
            OFFSET_POP;
            stage = "ui";
            glClearColor(0.18f, 0.18f, 0.18f, 1.0f);
        }
        else if (stage == "ui")
            exit(0);
        break;
    case 32: // space键
        camera.center = vec3::ZERO;
        break;
    case 'M': // 点击最大化按钮
    case 'm': 
        if (b_fullscreen) {
            glutReshapeWindow(SCREENX, SCREENY); // 还原窗口大小
            b_fullscreen = false;
        }
        else {
            glutFullScreen(); // 最大化窗口
            b_fullscreen = true;
        }
    }
}

// -------------------------------------------------
// WINDOW RESIZE
// -------------------------------------------------
void reshape(int w, int h) 
{
    int centerX = w / 2;
    int centerY = h / 2;
    OFFSET_X = (centerX - glutGet(GLUT_WINDOW_WIDTH) / 2);
    OFFSET_Y = (centerY - glutGet(GLUT_WINDOW_HEIGHT) / 2);

    if(w == 1842 && h == 1057)
    {
        glutFullScreen(); // 最大化窗口
        b_fullscreen = true;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(w, h);

    camera.aspect = (float)w / h;

    glutPostRedisplay();
}

// *************************************************
// 主函数
// *************************************************
int main(int argc, char** argv)
{
#ifdef WINDOWS
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // 获取标准输出句柄
#endif
    // COSOCOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 可能是自定义函数

    // 读取配置文件
    {
        std::ifstream configFile("vis.ini");
        std::string line;
        if (configFile.is_open())
        {
            while (getline(configFile, line))
            {
                if (line.substr(0, 7) == "HOSTIP=")
                {
                    HOSTIP = line.substr(7);
                    break;
                }
            }
            configFile.close();
        }
    }

    PRINT("######### start http-server:" << HOSTIP << ":5088 #########");
    std::thread serverthread = std::thread{ servermain, 100 };

    HANDLE hSemaphore = CreateSemaphore(NULL, 0, 1, "RenderS");
    if (hSemaphore == NULL) {
        cout << "Semaphore creation failed!" << endl;
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE); // 请求多重采样

    // 获取当前投屏分辨率和屏幕坐标
    int screenCount = GetSystemMetrics(SM_CMONITORS); // 获取屏幕数量

    int windowX = 158; // 窗口左上角X坐标
    int windowY = 58; // 窗口左上角Y坐标

    if (screenCount >= 2)
    {
        // 获取右边屏幕的信息
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(MONITORINFO);
        HMONITOR hMonitor = MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTONEAREST);
        GetMonitorInfo(hMonitor, &monitorInfo);
        windowX = monitorInfo.rcMonitor.right + 588; // 窗口左上角X坐标
        windowY = monitorInfo.rcMonitor.top + 58; // 窗口左上角Y坐标

        // 如果主从屏幕颠倒了，则将窗口位置调整到主屏幕上
        MONITORINFO primaryMonitorInfo;
        primaryMonitorInfo.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY), &primaryMonitorInfo);
        if (monitorInfo.rcMonitor.left > monitorInfo.rcMonitor.right)
        {
            windowX = primaryMonitorInfo.rcMonitor.left + 588;
            windowY = primaryMonitorInfo.rcMonitor.top + 58;
        }
    }

    glutInitWindowPosition(windowX, windowY);
    glutInitWindowSize(SCREENX, SCREENY);
    glutCreateWindow((string("Pythagoras [ ") + HOSTIP + " : 5088 ]").c_str());

    // 加载图标文件
    HICON hIcon = (HICON)LoadImage(NULL, "imgui/main.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    if (hIcon != NULL) {
        // 设置窗口标题栏图标
        SendMessage(GetActiveWindow(), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(GetActiveWindow(), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

    // 启用多重采样
    glEnable(GL_MULTISAMPLE);

    glutDisplayFunc(display_ui);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glewInit();

    init();

    ReleaseSemaphore(hSemaphore, 1, NULL);

    glutTimerFunc(0, update, 0); // 动画更新！
    
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    {
        // 初始化字体模块
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(SCREENX, SCREENY);

        // 添加自定义字体，指定字体文件路径和字体大小
        ImFont* customFont = io.Fonts->AddFontFromFileTTF("FZQTJW.TTF", 16.0f);
        if (customFont == nullptr) {
            // 处理字体加载失败的情况
            std::cerr << "Failed to load font!" << std::endl;
        }
        else {
            // 将自定义字体设置为默认字体
            io.FontDefault = customFont;
        }
        // 调整颜色以增加美感并减弱对比度
        ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.4f); // 更柔和的标题背景色
        ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = ImVec4(0.4f, 0.4f, 0.4f, 0.7f); // 更柔和的激活标题背景色
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.5f); // 更柔和的窗口背景色

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.35f, 0.0f, 1.0f)); // 按钮颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.25f, 0.0f, 1.0f)); // 悬停按钮颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.25f, 0.0f, 1.0f)); // 激活按钮颜色
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.6f, 0.5f, 0.0f, 1.0f)); // 边框颜色
        ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.3f, 0.2f, 0.0f, 1.0f)); // 边框阴影颜色

        ImGui_ImplOpenGL3_CreateFontsTexture();

        LoadImguiBindings();
    }

    glutMainLoop();

    {
        glDeleteTextures(1, &screenTexture);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteProgram(blurShader);
    }
    {
        ImGui::DestroyContext();
        ImGui_ImplOpenGL3_DestroyDeviceObjects();
    }
    CloseHandle(hSemaphore);

    return 0;
}