#include "pch.h"
#include "Rmgroup.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

RmGroup::RmGroup(string command_name) : ICommand(std::move(command_name), "Removes a group from the groups list.", "/rmgroup <group>", { "rmgrp" }) {}

vector<AvailableCommandsPacket::OverloadData> RmGroup::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("group");
    builder._break();
    return builder.payload();
}

CommandResult RmGroup::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    // YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
    if (provider == "yaml")
    {
        vector<PPGroup> groups;
        YAML::Node node = YAML::LoadFile("plugins/PurePerms/groups.yml");
        for (auto gr : node)
        {
            groups.push_back(gr.as<PPGroup>());
        }
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.rmgroup") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty())
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            bool b;
            vector<PPGroup> res;
            for (auto g : groups)
            {
                if (g.getName() == c_arg[0])
                {
                    b = 1;
                    continue;
                }
                res.push_back(g);
            }
            if (b == 0)
            {
                pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Group " + str2wstr(c_arg[0]) + L" already exists."));
                return CommandResult::ERRORR;
            }
            node.reset();
            for (auto g : res)
                node.push_back(g);
            remove("plugins/PurePerms/groups.yml");
            ofstream fout("plugins/PurePerms/groups.yml");
            fout << node;
            fout.close();
            pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] Removed " + utf8_to_wstring(c_arg[0]) + L" from the group list successfully."));
            return CommandResult::SUCCESS;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty())
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            bool b = 0;
            vector<PPGroup> res;
            for (auto g : groups)
            {
                if (g.getName() == c_arg[0])
                {
                    b = 1;
                    continue;
                }
                res.push_back(g);
            }
            if (b == 0)
            {
                wcout << L"[PurePerms] Group " + str2wstr(c_arg[0]) + L" already exists." << endl;
                return CommandResult::ERRORR;
            }
            node.reset();
            for (auto g : res)
                node.push_back(g);
            remove("plugins/PurePerms/groups.yml");
            ofstream fout("plugins/PurePerms/groups.yml");
            fout << node;
            fout.close();
            cout << wstring_to_utf8(L"[PurePerms] Removed " + str2wstr(c_arg[0]) + L" from the group list successfully.") << endl;
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