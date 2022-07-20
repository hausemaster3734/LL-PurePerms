#include "PPSudo.h"
#include "PPGroup.h"
#include "NoeulAPI.h"
#include <ScheduleAPI.h>
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

PPSudo::PPSudo(string command_name) : ICommand(std::move(command_name), "Registers or logs into your Noeul account.", "/ppsudo <register/login/unregister>", { "psudo" }) {}

vector<AvailableCommandsPacket::OverloadData> PPSudo::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("operation");
    builder.addData("password");
    builder._break();
    return builder.payload();
}

CommandResult PPSudo::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.ppsudo") || checkPermission(pl_sender->getName(), "*")))
    {
        if (noeul_sixtyfour == 0)
        {
            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Noeul is disabled."));
            return CommandResult::ERRORR;
        }
        if (c_arg.size() == 0 || c_arg.size() == 1)
        {
            pl_sender->sendText("Alias used: " + alias_used);
            return CommandResult::INVALID_ARGS;
        }
        else if (c_arg[1].size() < noeul_minimum_pw_length)
        {
            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Your password does NOT meet minimum length " + str2wstr(to_string(noeul_minimum_pw_length)) + L" requirements."));
            return CommandResult::ERRORR;
        }
        if (c_arg[0] == "register")
        {
            YAML::Node player = YAML::LoadFile("plugins/PurePerms/players/" + pl_sender->getName() + ".yml");
            if (player["noeulPW"].as<string>() == "")
            {
                player["noeulPW"] = md5(c_arg[1]);
                db[pl_sender->getUuid()] = 1;
                ofstream fout("plugins/PurePerms/players/" + pl_sender->getName() + ".yml");
                fout << player;
                fout.close();
                pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] Your account has been successfully registered."));
                return CommandResult::SUCCESS;
            }
            else if (player["noeulPW"].as<string>() != "")
            {
                pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] This is account arleady exist."));
                return CommandResult::ERRORR;
            }
        }
        else if (c_arg[0] == "login")
        {
            YAML::Node player = YAML::LoadFile("plugins/PurePerms/players/" + pl_sender->getName() + ".yml");
            if (player["noeulPW"].as<string>() == "")
            {
                pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] This account is not registered."));
                return CommandResult::ERRORR;
            }
            else if (player["noeulPW"].as<string>() != "")
            {
                if (db[pl_sender->getUuid()] == 0)
                {
                    if (player["noeulPW"].as<string>() == md5(c_arg[1]))
                    {
                        db[pl_sender->getUuid()] = 1;
                        pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] You are now logged in."));
                        return CommandResult::SUCCESS;
                    }
                    else
                    {
                        pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Invalid password."));
                        return CommandResult::ERRORR;
                    }
                }
                else if (db[pl_sender->getUuid()] == 1)
                {
                    pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] You are now logged in."));
                    return CommandResult::SUCCESS;
                }
            }
        }
        else if (c_arg[0] == "unregister")
        {
            YAML::Node player = YAML::LoadFile("plugins/PurePerms/players/" + pl_sender->getName() + ".yml");
            if (player["noeulPW"].as<string>() == "")
            {
                pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] This account is not registered."));
                return CommandResult::ERRORR;
            }
            else if (player["noeulPW"].as<string>() != "")
            {
                if (player["noeulPW"].as<string>() == md5(c_arg[1]))
                {
                    player["noeulPW"] = "";
                    ofstream fout("plugins/PurePerms/players/" + pl_sender->getName() + ".yml");
                    fout << player;
                    fout.close();
                    map<string, bool> res;
                    for (auto v : db)
                    {
                        if (v.first == pl_sender->getUuid())
                        {
                            continue;
                        }
                        res[v.first] = v.second;
                    }
                    db = res;
                    pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] Your account has been successfully unregistered."));
                    return CommandResult::SUCCESS;
                }
                else
                {
                    pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Invalid password."));
                    return CommandResult::ERRORR;
                }
            }  
        }
    }
    else if (!pl_sender->isPlayer())
    {
        if (noeul_sixtyfour == 0)
        {
           cout << "[PurePerms] Noeul is disabled.\n";
           return CommandResult::ERRORR;
        }
        cout << "[PurePerms] This command should be only used in-game.\n";
        return CommandResult::ERRORR;
    }
    return CommandResult::FAILED_PERMISSION;
}