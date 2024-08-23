// -----------------------------------
// ��Ϣ��ʾ
// -----------------------------------
//#define NOTICE(msg)     notification::ShowNotification(msg)

namespace notification
{
    // �ṹ�����ڹ�����Ϣ��ʾ
    struct Notification {
        string message;
        float displayTime; // ��ʾʱ��
        float alpha;       // ͸����
        bool isVisible;    // �Ƿ�ɼ�
    };

    // ����һ��ȫ����Ϣ��ʾ
    Notification notification = { "", 0.0f, 1.0f, false };

    // ����������ʾ��Ϣ��ʾ
    void ShowNotification(const char* message, float duration = 3.0f) {
        notification.message = message;
        notification.displayTime = duration;
        notification.alpha = 1.0f;
        notification.isVisible = true;
    }

    // ����Ⱦѭ��
    void RenderLoop() 
    {
        // ������Ϣ��ʾ����ʾ
        if (notification.isVisible) {
            // ������Ⱦ
            ImGui::NewFrame();
            // ������ʾʱ��
            notification.displayTime -= ImGui::GetIO().DeltaTime;

            // ���ʱ�䵽����ʼ����͸����
            if (notification.displayTime <= 0.0f) {
                notification.alpha = blend(notification.alpha, 0.0, 0.5); // ������ʧ
                if (notification.alpha <= 0.018f) {
                    notification.isVisible = false; // ������Ϣ��ʾ
                }
            }

            // ������Ϣ��ʾ��λ�ú���ʽ
            ImGui::SetNextWindowBgAlpha(notification.alpha);
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, 30), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
            ImGui::Begin("Notification", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

            ImGui::Text("   %s   ", notification.message.c_str());
            ImGui::End();

            // ��ȾImGui
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