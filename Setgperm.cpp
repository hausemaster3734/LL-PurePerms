#include "Setgperm.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

Setgperm::Setgperm(string command_name) : ICommand(std::move(command_name), "Adds a permission to the group.", "/setgperm <group> <permission>", {"sgperm"}) {}

vector<AvailableCommandsPacket::OverloadData> Setgperm::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("group");
    builder.addData("permission");
    builder._break();
    return builder.payload();
}

CommandResult Setgperm::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    if (provider == "yaml")
    {
        vector<PPGroup> groups;
        YAML::Node node = YAML::LoadFile("plugins/PurePerms/groups.yml");
        for (auto gr : node)
        {
            groups.push_back(gr.as<PPGroup>());
        }
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.setgperm") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty() || c_arg.size()==1)
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            for (int i = 0; i < groups.size(); ++i)
            {
                if (groups[i].getName() == c_arg[0])
                {
                    groups[i].addPermission(c_arg[1]);
                    node.reset();
                    for (auto g : groups)
                        node.push_back(g);
                    remove("plugins/PurePerms/groups.yml");
                    ofstream fout("plugins/PurePerms/groups.yml");
                    fout << node;
                    fout.close();
                    pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] Added permission " + str2wstr(c_arg[1]) + L" to the group successfully."));
                    return CommandResult::SUCCESS;
                }
            }
            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Group " + str2wstr(c_arg[0]) + L" does NOT exist."));
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
                if (groups[i].getName() == c_arg[0])
                {
                    groups[i].addPermission(c_arg[1]);
                    node.reset();
                    for (auto g : groups)
                        node.push_back(g);
                    remove("plugins/PurePerms/groups.yml");
                    ofstream fout("plugins/PurePerms/groups.yml");
                    fout << node;
                    fout.close();
                    wcout << L"[PurePerms] Added permission " + str2wstr(c_arg[1]) + L" to the group successfully.\n";
                    return CommandResult::SUCCESS;
                }
            }
            wcout << L"[PurePerms] Group " + str2wstr(c_arg[0]) + L" does NOT exist.\n";
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