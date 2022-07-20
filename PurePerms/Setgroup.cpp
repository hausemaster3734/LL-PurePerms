#include "Setgroup.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <MC/Level.hpp>
#include <Utils/StringHelper.h>

Setgroup::Setgroup(string command_name) : ICommand(std::move(command_name), "Sets group for the user.", "/setgroup <player> <group> <exptime>", { "sgroup" }) {}

vector<AvailableCommandsPacket::OverloadData> Setgroup::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("player");
    builder.addData("group");
    builder.addData("exptime");
    builder._break();
    return builder.payload();
}

CommandResult Setgroup::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    if (provider == "yaml")
    {
        vector<PPGroup> groups;
        YAML::Node node = YAML::LoadFile("plugins/PurePerms/groups.yml");
        for (auto gr : node)
        {
            groups.push_back(gr.as<PPGroup>());
        }
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.setgroup") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty() || c_arg.size() == 1)
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            for (int i = 0; i < groups.size(); ++i)
            {
                if (groups[i].getName() == c_arg[1])
                {
                    for (auto v : superadmin_ranks)
                    {
                        if (c_arg[1] == v)
                        {
                            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] You don't have permission to set a player's group to " + str2wstr(c_arg[1])));
                            return CommandResult::ERRORR;
                        }
                    }
                    auto player = getPlayer(c_arg[0]);
                    if (player.getNickname() == "")
                    {
                        pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Player " + str2wstr(c_arg[0]) + L" does NOT exist.\n"));
                        return CommandResult::ERRORR;
                    }
                    for (auto v : superadmin_ranks)
                    {
                        if (player.getGroup().getName() == v)
                        {
                            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] You don't have permission to change group of a player with group " + str2wstr(c_arg[1])));
                            return CommandResult::ERRORR;
                        }
                    }
                    player.setGroup(c_arg[1]);
                    player.setExpTime(atoi(c_arg[2].c_str()));
                    YAML::Node playerr;
                    playerr = player;
                    ofstream fout("plugins/PurePerms/players/" + c_arg[0] + ".yml");
                    fout << playerr;
                    fout.close();
                    pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] Added " + str2wstr(c_arg[0]) + L" to the group successfully."));
                    for (auto p : Level::getAllPlayers())
                    {
                        if (p->getName() == c_arg[0])
                        {
                            p->sendText(wstring_to_utf8(L"§a[PurePerms] Your group has been changed to a / an " + str2wstr(c_arg[1])));
                            break;
                        }
                    }
                    return CommandResult::SUCCESS;
                }
            }
            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Group " + str2wstr(c_arg[1]) + L" does NOT exist."));
            return CommandResult::ERRORR;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty() || c_arg.size() == 1)
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            for (int i = 0; i < groups.size(); ++i)
            {
                if (groups[i].getName() == c_arg[1])
                {
                    auto player = getPlayer(c_arg[0]);
                    if (player.getNickname() == "")
                    {
                        cout << "[PurePerms] Player " + c_arg[0] + " does NOT exist.\n";
                        return CommandResult::ERRORR;
                    }
                    player.setGroup(c_arg[1]);
                    player.setExpTime(atoi(c_arg[2].c_str()));
                    YAML::Node playerr;
                    playerr = player;
                    ofstream fout("plugins/PurePerms/players/" + c_arg[0] + ".yml");
                    fout << playerr;
                    fout.close();
                    cout << "[PurePerms] Added " + c_arg[0] + " to the group successfully.\n";
                    for (auto p : Level::getAllPlayers())
                    {
                        if (p->getName() == c_arg[0])
                        {
                            cout << "[PurePerms] Your group has been changed to a / an " + c_arg[1] + "\n";
                            break;
                        }
                    }
                    return CommandResult::SUCCESS;
                }
            }
            cout << "[PurePerms] Group " + c_arg[1] + " does NOT exist.\n";
            return CommandResult::ERRORR;
        }
        return CommandResult::FAILED_PERMISSION;
    }
    else if (provider == "json")
    {
        //Алексе
        //PS Пример с правами и отделения ввода с консоли и ввода с игры смотри выше
    }
}