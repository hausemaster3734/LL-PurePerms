#include "Addgroup.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

AddGroup::AddGroup(string command_name) : ICommand(std::move(command_name), "Adds a new group to the groups list.", "/addgroup <group>", { "adgr" }) {}

vector<AvailableCommandsPacket::OverloadData> AddGroup::getOverloads() 
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("group");
    builder._break();
    return builder.payload();
}

CommandResult AddGroup::execute(Player* pl_sender, string alias_used, vector<string> c_arg) 
{
    if (provider == "yaml")
    {
        vector<PPGroup> groups;
        YAML::Node node = YAML::LoadFile("plugins/PurePerms/groups.yml");
        for (auto gr : node)
        {
            groups.push_back(gr.as<PPGroup>());
        }
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.addgroup") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty())
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            for (auto gr : node)
            {
                if (gr.as<PPGroup>().getName() == c_arg[0])
                {
                    pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Group " + str2wstr(c_arg[0]) + L" already exists."));
                    return CommandResult::ERRORR;
                }
            }
            PPGroup group(c_arg[0], "", 0, {}, {});
            groups.push_back(group);
            node.reset();
            for (auto g : groups)
                node.push_back(g);
            remove("plugins/PurePerms/groups.yml");
            ofstream fout("plugins/PurePerms/groups.yml");
            fout << node;
            fout.close();
            pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] Added " + utf8_to_wstring(c_arg[0]) + L" to the group list successfully."));
            return CommandResult::SUCCESS;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty())
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            for (auto gr : node)
            {
                if (gr.as<PPGroup>().getName() == c_arg[0])
                {
                   cerr << wstring_to_utf8(L"[PurePerms] Group " + str2wstr(c_arg[0]) + L" already exists..") << endl;
                   return CommandResult::ERRORR;
                }
            }
            PPGroup group(c_arg[0], "", 0, {}, {});
            groups.push_back(group);
            node.reset();
            for (auto g : groups)
                node.push_back(g);
            remove("plugins/PurePerms/groups.yml");
            ofstream fout("plugins/PurePerms/groups.yml");
            fout << node;
            fout.close();
            cerr << wstring_to_utf8(L"[PurePerms] Added " + str2wstr(c_arg[0]) + L" to the group list successfully.") << endl;
            return CommandResult::SUCCESS;
        }
        return CommandResult::FAILED_PERMISSION;
    }
    else if (provider == "json")
    {
        //Алексе
        //PS Пример с правами и отделения ввода с консоли и ввода с игры смотри выше
    }
}