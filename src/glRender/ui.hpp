// -----------------------------------
// 消息提示
// -----------------------------------
//#define NOTICE(msg)     notification::ShowNotification(msg)

namespace notification
{
    // 结构体用于管理消息提示
    struct Notification {
        string message;
        float displayTime; // 显示时间
        float alpha;       // 透明度
        bool isVisible;    // 是否可见
    };

    // 创建一个全局消息提示
    Notification notification = { "", 0.0f, 1.0f, false };

    // 函数用于显示消息提示
    void ShowNotification(const char* message, float duration = 3.0f) {
        notification.message = message;
        notification.displayTime = duration;
        notification.alpha = 1.0f;
        notification.isVisible = true;
    }

    // 主渲染循环
    void RenderLoop() 
    {
        // 处理消息提示的显示
        if (notification.isVisible) {
            // 清理渲染
            ImGui::NewFrame();
            // 更新显示时间
            notification.displayTime -= ImGui::GetIO().DeltaTime;

            // 如果时间到，开始减少透明度
            if (notification.displayTime <= 0.0f) {
                notification.alpha = blend(notification.alpha, 0.0, 0.5); // 慢慢消失
                if (notification.alpha <= 0.018f) {
                    notification.isVisible = false; // 隐藏消息提示
                }
            }

            // 设置消息提示的位置和样式
            ImGui::SetNextWindowBgAlpha(notification.alpha);
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, 30), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
            ImGui::Begin("Notification", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

            ImGui::Text("   %s   ", notification.message.c_str());
            ImGui::End();

            // 渲染ImGui
            ImGui::Render();

            glutPostRedisplay();
        }
        else
        {
            if (!GTable::msgqueue.empty())
            {
                ShowNotification(GTable::msgqueue.back().c_str());
                GTable::msgqueue.pop_back();
            }
        }
    }
}