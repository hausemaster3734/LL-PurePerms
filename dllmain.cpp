
// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "Core.h"
#include <LLAPI.h>
#include <ScheduleAPI.h>
#include <yaml-cpp/yaml.h>
#include "PPGroup.h"
#include "PPPlayer.h"
#include "EventListener.h"
#include <regex>
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
bool noeul_sixtyfour;
int noeul_minimum_pw_length;
bool ret = 0;
map<string,bool> db;
ScheduleTask task,group_task;

void grouptask()
{
    string path = "plugins/PurePerms/players";
    for (const auto& file : std::filesystem::directory_iterator::directory_iterator(path))
    {
        string file1 = split(file.path().string(), "\\")[1];
        YAML::Node player = YAML::LoadFile("plugins/PurePerms/players/" + file1);
        PPPlayer pl = player.as<PPPlayer>();
        if (pl.getExpTime() > 0)
        {
            pl.setExpTime(pl.getExpTime() - 60);
        }
        else if (pl.getExpTime() == -1)
        {
            continue;
        }
        else if (pl.getExpTime() <= 0 && pl.getExpTime() != -9999)
        {
            YAML::Node grs = YAML::LoadFile("plugins/PurePerms/groups.yml");
            string def_group;
            for (auto pg : grs)
            {
                if (pg.as<PPGroup>().isDefault == 1)
                {
                    def_group = pg.as<PPGroup>().getName();
                    break;
                }
            }
            pl.setGroup(def_group);
            pl.setExpTime(-1);
        }
        player.reset();
        player = pl;
        ofstream fout("plugins/PurePerms/players/" + pl.getNickname() + ".yml");
        fout << player;
        fout.close();
    }
}

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
        config["enable-noeul-sixtyfour"] = false;
        config["noeul-minimum-pw-length"] = 6;
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
        provider = config["provider"].as<string>();
        disable_op = config["disable-op"].as<bool>();
        noeul_sixtyfour = config["enable-noeul-sixtyfour"].as<bool>();
        noeul_minimum_pw_length = config["noeul-minimum-pw-length"].as<int>();
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
                PPGroup group3("Admin", "admin", 0, { "Moder","VIP" }, { });
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
        else if (provider == "json")
        {
            //Алексе,тут ты своё пишешь
        }
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
    }
    Event::PlayerJoinEvent::subscribe([](const Event::PlayerJoinEvent& ev) 
    {
            if (provider == "yaml")
            {
                ifstream fin("plugins/PurePerms/players/" + ev.mPlayer->getName() + ".yml");
                if (!fin.is_open())
                {
                    fin.close();
                    YAML::Node grs = YAML::LoadFile("plugins/PurePerms/groups.yml");
                    YAML::Node player;
                    PPPlayer ppl;
                    ppl.setNickname(ev.mPlayer->getName());
                    string def_group;
                    for (auto pg : grs)
                    {
                        if (pg.as<PPGroup>().isDefault == 1)
                        {
                            def_group = pg.as<PPGroup>().getName();
                            break;
                        }
                    }
                    ppl.setGroup(def_group);
                    ppl.addPermission("pperms.command.ppsudo");
                    ppl.setPassword("");
                    ppl.setTime(-1);
                    ppl.setExpTime(-1);
                    player = ppl;
                    ofstream fout("plugins/PurePerms/players/" + ev.mPlayer->getName() + ".yml");
                    fout << player;
                    fout.close();
                }
                fin.close();
            }
            else if (provider == "json")
            {
                //Алексе
            }
            if (noeul_sixtyfour)
            {
                ev.mPlayer->sendText(wstring_to_utf8(L"§a[PurePerms] This server uses Noeul v1.0.0 to protect your account."));
                ev.mPlayer->sendText(wstring_to_utf8(L"§a[PurePerms] You must authenticate your account first."));
                ev.mPlayer->sendText(wstring_to_utf8(L"§a[PurePerms] Please register or login to your account using /ppsudo command"));
                if (provider == "yaml")
                {
                    YAML::Node player = YAML::LoadFile("plugins/PurePerms/players/" + ev.mPlayer->getName() + ".yml");
                    if (!player.IsNull())
                    {
                        if (player["noeulPW"].as<string>() != "" && db.find(ev.mPlayer->getUuid()) == db.end())
                        {
                            db[ev.mPlayer->getUuid()] = 0;
                        }
                    }
                }
                else if (provider == "json")
                {
                    //Алексе
                }
            }
            return 1;
    });
    Event::PlayerLeftEvent::subscribe([&](const Event::PlayerLeftEvent& ev) 
    {
            db[ev.mPlayer->getUuid()] = 0;
            return 1;
    });
    Event::PlayerCmdEvent::subscribe([&](const Event::PlayerCmdEvent& ev) 
    {
            if (disable_op)
            {
                regex r("op");
                smatch sm;
                if (regex_search(ev.mCommand, sm, r))
                {
                    ev.mPlayer->sendText(wstring_to_utf8(L"§4[PurePerms] This is command disabled for player's on server."));
                    return false;
                }
            }
            return true;
    });
    auto* loader = new CommandAPI();
    loader->onEnable();
    task = Schedule::repeat([&]()
        {
            if (noeul_sixtyfour)
            {
                auto pls = Level::getAllPlayers();
                for (auto p : pls)
                {
                    if (provider == "yaml")
                    {
                        YAML::Node player = YAML::LoadFile("plugins/PurePerms/players/" + p->getName() + ".yml");
                        if (!player.IsNull())
                        {
                            if (player["noeulPW"].as<string>() == "")
                            {
                                continue;
                            }
                            else if (player["noeulPW"].as<string>() != "")
                            {
                                if (db.find(p->getUuid()) == db.end())
                                {
                                    continue;
                                }
                                if (db[p->getUuid()] == 0)
                                {
                                    string pt = std::filesystem::temp_directory_path().string();
                                    ifstream fin(pt + "/" + p->getName() + ".yml");
                                    if (!fin.is_open())
                                    {
                                        ofstream fout(pt + "/" + p->getName() + ".yml");
                                        fout << player;
                                        fout.close();
                                    }
                                    player["group"] = "";
                                    vector<string> perms;
                                    for (auto perm : player["permissions"].as<vector<string>>())
                                    {
                                        if (perm == "pperms.command.ppsudo")
                                        {
                                            perms.push_back(perm);
                                            break;
                                        }
                                    }
                                    player["permissions"] = perms;
                                    ofstream fout("plugins/PurePerms/players/" + p->getName() + ".yml");
                                    fout << player;
                                    fout.close();
                                }
                                else if (db[p->getUuid()] == 1)
                                {
                                    string pt = std::filesystem::temp_directory_path().string();
                                    ifstream fin(pt + "/" + p->getName() + ".yml");
                                    if (fin.is_open())
                                    {
                                        fin.close();
                                        YAML::Node player1 = YAML::LoadFile(pt + "/" + p->getName() + ".yml");
                                        ofstream fout("plugins/PurePerms/players/" + p->getName() + ".yml");
                                        fout << player1;
                                        fout.close();
                                        string pts = pt + "/" + p->getName() + ".yml";
                                        remove(pts.c_str());
                                    }
                                    fin.close();
                                }
                            }
                        }
                        else
                        {
                            return;
                        }
                    }
                    else if (provider == "json")
                    {
                        //TODO Алексе,тут ты уже кодишь
                    }
                }
                return;
            }
            return;
        }, 20);
    group_task = Schedule::repeat(grouptask, 1200);
}