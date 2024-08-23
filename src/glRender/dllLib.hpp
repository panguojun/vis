 /**
 *              调用外部dll
 *              DLL中提供具体事务内容
 *              本工程只负责绘制
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
        // 读取配置文件
        std::ifstream configFile("config.ini");
        if (!configFile.is_open())
        {
            std::cerr << "\nFailed to open config file" << std::endl;
            return 1;
        }
        // 解析 DLL 列表
        
        std::string line;
        while (std::getline(configFile, line))
        {
            if (!line.empty())
            {
                dllList.push_back(line);
            }
        }
        // 加载 DLL
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
        // 卸载 DLL
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
        // 获取接口函数地址并调用
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
