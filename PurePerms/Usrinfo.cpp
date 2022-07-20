#include "Usrinfo.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <MC/Level.hpp>
#include <Utils/StringHelper.h>

Usrinfo::Usrinfo(string command_name) : ICommand(std::move(command_name), "Shows info about a user.", "/usrinfo <user>", { "uinfo" }) {}

vector<AvailableCommandsPacket::OverloadData> Usrinfo::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("user");
    builder._break();
    return builder.payload();
}

CommandResult Usrinfo::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    if (provider == "yaml")
    {
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.usrinfo") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty())
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            ifstream f("plugins/PurePerms/players/" + c_arg[0] + ".yml");
            if (!f.is_open())
            {
                f.close();
                pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Player " + str2wstr(c_arg[0]) + L" does NOT exist."));
                return CommandResult::ERRORR;
            }
            f.close();
            PPPlayer pls;
            YAML::Node player = YAML::LoadFile("plugins/PurePerms/players/" + c_arg[0] + ".yml");
            pls = player.as<PPPlayer>();
            wstring out = L"§a[PurePerms] -- -- User Information for " + str2wstr(c_arg[0]) + L" --\n";
            out += L"§a[PurePerms] USERNAME: §b§2" + str2wstr(pls.getNickname()) + L"\n";
            string status = "OFFLINE..";
            Player* pl;
            for (auto p : Level::getAllPlayers())
            {
                if (p->getName() == c_arg[0])
                {
                    pl = p;
                    status = "ONLINE!";
                    break; 
                }
            }
            out += L"§a[PurePerms] STATUS: §b§2" + str2wstr(status) + L"\n";
            bool b = false;
            for (auto p : Level::getAllPlayers())
            {
                if (p->getName() == c_arg[0])
                {
                    b = 1;
                    out += L"§a[PurePerms] IP: §b§2" + str2wstr(split(pl->getIP(), ":")[0]) + L"\n";
                    out += L"§a[PurePerms] UNIQUE ID: §b§2" + str2wstr(pl->getUuid()) + L"\n";
                    break;
                }
            }
            if (!b)
            {
                out += L"§a[PurePerms] IP: §b§2UNKOWN\n";
                out += L"§a[PurePerms] UNIQUE ID: §b§2UNKOWN\n";
            }
            out += L"§a[PurePerms] GROUP: §b§2" + str2wstr(pls.getGroup().getName()) + L"\n";
            out += L"§a[PurePerms] ExpTime: §b§2" + str2wstr(to_string(pls.getExpTime())) + L"\n";
            pl_sender->sendText(wstring_to_utf8(out));
            return CommandResult::SUCCESS;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty())
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            ifstream f("plugins/PurePerms/players/" + c_arg[0] + ".yml");
            if (!f.is_open())
            {
                f.close();
                cout << "[PurePerms] Player " + c_arg[0] + " does NOT exist.\n";
                return CommandResult::ERRORR;
            }
            f.close();
            PPPlayer pls;
            YAML::Node player = YAML::LoadFile("plugins/PurePerms/players/" + c_arg[0] + ".yml");
            pls = player.as<PPPlayer>();
            wstring out = L"[PurePerms] -- -- User Information for " + str2wstr(c_arg[0]) + L" --\n";
            out += L"[PurePerms] USERNAME: " + str2wstr(pls.getNickname()) + L"\n";
            string status = "OFFLINE..";
            Player* pl;
            for (auto p : Level::getAllPlayers())
            {
                if (p->getName() == c_arg[0])
                {
                    pl = p;
                    status = "ONLINE!";
                    break;
                }
            }
            out += L"[PurePerms] STATUS: " + str2wstr(status) + L"\n";
            bool b = false;
            for (auto p : Level::getAllPlayers())
            {
                if (p->getName() == c_arg[0])
                {
                    b = 1;
                    out += L"[PurePerms] IP: 2" + str2wstr(split(pl->getIP(), ":")[0]) + L"\n";
                    out += L"[PurePerms] UNIQUE ID: " + str2wstr(pl->getUuid()) + L"\n";
                    break;
                }
            }
            if (!b)
            {
                out += L"[PurePerms] IP: UNKOWN\n";
                out += L"[PurePerms] UNIQUE ID: UNKOWN\n";
            }
            out += L"[PurePerms] GROUP: " + str2wstr(pls.getGroup().getName()) + L"\n";
            out += L"[PurePerms] ExpTime: " + str2wstr(to_string(pls.getExpTime())) + L"\n";
            std::wcout << out << endl;
            return CommandResult::SUCCESS;
        }
    }
    else if (provider == "json")
    {
        //TODO Àëåêñå
    }
    return CommandResult::FAILED_PERMISSION;
}