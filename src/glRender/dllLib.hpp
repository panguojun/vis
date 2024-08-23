 /**
 *              �����ⲿdll
 *              DLL���ṩ������������
 *              ������ֻ�������
 */

namespace dllLib
{

    std::vector<std::string> dllList;
    std::vector<HINSTANCE> hDLLs;

    void dllmsg(const std::string& msg, const std::string& body)
    {
        std::string out;
        CMD::onmsg(msg, body);
    }

    bool load()
    {
        // ��ȡ�����ļ�
        std::ifstream configFile("config.ini");
        if (!configFile.is_open())
        {
            std::cerr << "\nFailed to open config file" << std::endl;
            return 1;
        }
        // ���� DLL �б�
        
        std::string line;
        while (std::getline(configFile, line))
        {
            if (!line.empty())
            {
                dllList.push_back(line);
            }
        }
        // ���� DLL
        for (const auto& dllName : dllList)
        {
            HINSTANCE hDLL = LoadLibrary(dllName.c_str());
            if (hDLL == NULL)
            {
                std::cerr << "Failed to load DLL: " << dllName << std::endl;
                continue;
            }
            hDLLs.push_back(hDLL);

            InitFunc Init = (InitFunc)GetProcAddress(hDLL, "Init");
            if (Init != NULL)
            {
               /* Init(CMD::onmsg,
                    shape2d::drawCircle
                );*/
            }
        }
        return true;
    }
    void free()
    {
        // ж�� DLL
        for (const auto& hDLL : hDLLs)
        {
            ExitFunc Exit = (ExitFunc)GetProcAddress(hDLL, "Exit");

            if (Exit != NULL)
            {
                Exit();
            }
            FreeLibrary(hDLL);
        }
        hDLLs.clear();
    }
    bool draw()
    {
        // ��ȡ�ӿں�����ַ������
        for (const auto& hDLL : hDLLs)
        {
            DrawFunc Update = (DrawFunc)GetProcAddress(hDLL, "Draw");

            if (Update != NULL)
            {
                Update(0);

                return true;
            }
        }
        return false;
    }
}
