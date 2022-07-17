
// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "Core.h"
#include <LLAPI.h>
#include <yaml-cpp/yaml.h>
#include "PPGroup.h"
#include "PPPlayer.h"
#include "EventListener.h"
using namespace std;
#pragma comment(lib,"bedrock_server_api.lib")
#pragma comment(lib,"bedrock_server_var.lib")
#pragma comment(lib,"Lib/LiteLoader.lib")
#pragma comment(lib,"yaml-cpp.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LL::registerPlugin("PurePerms", "Порт PMMP версии PurePerms", LL::Version(0, 0, 0), "","",{});
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void PluginInit();

extern "C" {
    // Do something after all the plugins loaded
    _declspec(dllexport) void onPostInit() {
        std::ios::sync_with_stdio(false);
        PluginInit();
    }
}

namespace fs = std::filesystem;

string provider;
vector<string> superadmin_ranks;
bool disable_op;

void PluginInit()
{
    fs::create_directory("plugins/PurePerms");
    fs::create_directory("plugins/PurePerms/players");
    auto path = std::filesystem::temp_directory_path().string();
    vector<string> res;
    res.push_back("pperms.command.addgroup");
    res.push_back("pperms.command.addparent");
    res.push_back("pperms.command.defgroup");
    res.push_back("pperms.command.fperms");
    res.push_back("pperms.command.groups");
    res.push_back("pperms.command.grpinfo");
    res.push_back("pperms.command.listgperms");
    res.push_back("pperms.command.listuperms");
    res.push_back("pperms.command.ppinfo");
    res.push_back("pperms.command.ppsudo");
    res.push_back("pperms.command.ppreload");
    res.push_back("pperms.command.rmgroup");
    res.push_back("pperms.command.rmparent");
    res.push_back("pperms.command.setgperm");
    res.push_back("pperms.command.setgroup");
    res.push_back("pperms.command.setuperm");
    res.push_back("pperms.command.unsetgperm");
    res.push_back("pperms.command.unsetuperm");
    res.push_back("pperms.command.usrinfo");
    ifstream fin(path + "/PurePerms.tmp");
    if (!fin.is_open())
    {
        fin.close();
        ofstream fout(path + "/PurePerms.tmp");
        for (auto s : res)
            fout << s << endl;
        fout.close();
    }
    else
        fin.close();
    fin.open("plugins/PurePerms/config.yml");
    if (!fin.is_open())
    {
        fin.close();
        YAML::Node config;
        config["provider"] = "yaml";
        config["disable-op"] = true;
        config["superadmin-ranks"].push_back("OP");
        ofstream fout1("plugins/PurePerms/config.yml");
        fout1 << config;
        fout1.close();
        provider = "yaml";
        disable_op = 1;
        superadmin_ranks.push_back("OP");
    }
    else
    {
        fin.close();
        YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
        provider = "yaml";
        disable_op = 1;
        superadmin_ranks = config["superadmin-ranks"].as<vector<string>>();
    }
    try
    {
        if (provider == "yaml")
        {
            fin.open("plugins/PurePerms/groups.yml");
            if (!fin.is_open())
            {
                fin.close();
                PPGroup group("Player", "player", 1, { }, {  });
                PPGroup group1("VIP", "vip", 0, { "Player" }, {});
                PPGroup group2("Moder", "moder", 0, { "VIP" }, { });
                PPGroup group3("Admin", "admin", 0, { "Moder","VIP" }, {  });
                PPGroup group4("OP", "op", 0, { }, { "*" });
                groups.push_back(&group);
                YAML::Node gr;
                gr.push_back(group);
                gr.push_back(group1);
                gr.push_back(group2);
                gr.push_back(group3);
                gr.push_back(group4);
                ofstream fout("plugins/PurePerms/groups.yml");
                fout << gr;
                fout.close();
            }
            else
                fin.close();
        }
        else
        {
            //Алексе,тут ты своё пишешь
        }
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
    }
    auto* loader = new CommandAPI();
    loader->onEnable();
}