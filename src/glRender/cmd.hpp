/**
*               【命令】
*/
extern std::string   CLIENT;
extern const char* RunString(const char* szLua);
extern std::string lua_net_string = "";
namespace CMD
{
    void parse_command(const std::string& commands)
    {
        auto cmdstrs = STR::split(commands, '\n');
        for (auto& command : cmdstrs)
        {
            std::istringstream iss(command);
            std::string cmd, params;
            std::getline(iss, cmd, ' ');
            std::getline(iss, params, ' ');
            
            if (cmd == "stage")
            {
                params = STR::getparam(params.c_str());
                stage = params;
            }
            else if (cmd == "net")
            {
                STR::removeTrailingSpaces(params.data());
                SEND_MSG(CLIENT, 8081, "/cmd", params);
                PRINT("net cmd: " << params)
            }
            else if (cmd == "phg")
            {
                doPHG((params).c_str());
            }
            else if (cmd == "lua")
            {
                RunString((params).c_str());
            }
        }
    }
    void onmsg(const std::string& msg, const std::multimap<std::string, std::string>& params, std::string& out)
    {
        if (msg == "get")
        {
            strVSteam2d::on_get(params, out);

            glutPostRedisplay();
        }
    }
    void onmsg(const std::string& msg, const std::string& body)
    {
        //PRINT("msg=" << body)
        if (msg == "cmd")
        {
            if (!body.empty())
            {
                CMD::parse_command(body);

                glutPostRedisplay();
            }
        }
        else if (msg == "cmd2d")
        {
            if (!body.empty())
            {
                OFFSET_PUSH;
                stage = "2d";
                substage = "";
               // strVSteam2d::commands.clear();
                std::istringstream iss(body);
                auto cmdstrs = STR::split(body, '\n');
                for (auto& s : cmdstrs)
                {
                    if (s == "clear")
                        strVSteam2d::commands.clear();
                    else
                        strVSteam2d::commands.push_back(s);
                }

                glutPostRedisplay();
            }
        }
        else if (msg == "cmd3d")
        {
            if (!body.empty())
            {
                OFFSET_PUSH;
                stage = "3d";

                std::istringstream iss(body);
                auto cmdstrs = STR::split(body, '\n');
                for (auto& s : cmdstrs)
                {
                    if (s == "clear")
                    {
                        PM::smesh_list.clear();
                        //resetsm();
                        strVSteam3d::commands.clear();
                    }
                    if (s.substr(0,3) == "mdf")
                    {
                        size_t pos = s.find("BoolOper");

                        if (pos != std::string::npos) {
                            std::string rest = s.substr(pos + 8); // 8 是 "BoolOper " 的长度
                            PRINTV(rest);
                            doPHG(rest.c_str());
                            PM::setup_mesh_data();
                            substage = "phg";
                            glutPostRedisplay();
                            return;
                        }
                        else
                        {
                            auto mdfparams = STR::split(s, ' ');
                            for (auto& param : mdfparams) {
                                PRINTV(param);
                            }
                        }
                    }
                    else
                        strVSteam3d::commands.push_back(s);
                }
                
                substage = "";

                PM::smesh_list.push_back(make_shared<submesh>());
                gcur_sm = &(*PM::smesh_list.back());

                strVSteam3d::docmd();
                PM::setup_sm(CUR_SM);
                RESTORESM;

                glutPostRedisplay();
            }
        }
        else if (msg == "lua")
        {
            // 解析HTTP请求中的body部分，获取draw命令
            std::string drawCmd;
            drawCmd = body;

            // 解析draw命令，执行相应操作
            /*if (!drawCmd.empty())
            {
                OFFSET_PUSH;
                stage = "3d";
                doPHG(("lua([[" + drawCmd + "]]);").c_str());

                PM::setup_mesh_data();
              
                glutPostRedisplay();
            }*/
            if (!drawCmd.empty())
            {
                OFFSET_PUSH;
                stage = "ui";

                //RunString((drawCmd).c_str());
                lua_net_string = drawCmd;
                glutPostRedisplay();
            }
        }
        else if (msg == "phg")
        {
            // 解析draw命令，执行相应操作
            if (!body.empty())
            {
                OFFSET_PUSH;
                if(stage == "login")
                {
                    glutReshapeWindow(SCREENX, SCREENY);
                }
                stage = "3d";
                substage = "phg";

                //if (body.find("{") != std::string::npos)
                //{// “完整”的PHG则自动清理
                //    resetsm(CUR_SM);
                //}
                doPHG((body + ";\0").c_str());

                //// DUMP
                //for (auto const& pair : GTable::intmap) {
                //    PRINT("int Key: " << pair.first << " Value: " << pair.second);
                //}

                GLOBAL_SCALE = GTable::floatmap["GLOBAL_SCALE"];
                if (GLOBAL_SCALE == 0)
                    GLOBAL_SCALE = 1;

                PM::setup_mesh_data();
                glutPostRedisplay();
               
            }
        }
        else if (msg == "hlr")
        {
            if (!body.empty())
            {
                stage = "2d";
                substage = "hlr";

                doPHG((body + ";").c_str());

                hlr_map.clear();
                drawhlr();
                glutPostRedisplay();
            }
        }
        else if (msg == "shader")
        {
            // todo
            glutPostRedisplay();
        }
    }
}