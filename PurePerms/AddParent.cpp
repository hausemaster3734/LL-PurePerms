#include "pch.h"
#include "AddParent.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

AddParent::AddParent(string command_name) : ICommand(std::move(command_name), "Adds a group to another group inheritance list", "/addparent <target_group>  <parent_group>", { "adpt" }) {}

vector<AvailableCommandsPacket::OverloadData> AddParent::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("target_group");
    builder.addData("parent_group");
    builder._break();
    return builder.payload();
}

CommandResult AddParent::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    vector<PPGroup> groups;
    if (provider == "yaml")
    {
        if (c_arg.empty() || c_arg.size() == 1)
        {
            pl_sender->sendText("Alias used: " + alias_used);
            return CommandResult::INVALID_ARGS;
        }
        YAML::Node node = YAML::LoadFile("plugins/PurePerms/groups.yml");
        for (auto gr : node)
        {
            groups.push_back(gr.as<PPGroup>());
        }
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.addparent") || checkPermission(pl_sender->getName(), "*")))
        {
            for (int i = 0; i < groups.size(); ++i)
            {
                if (groups[i].getName() == c_arg[0])
                {
                    for (auto gs : groups[i].getInheritances())
                    {
                        if (gs == c_arg[1])
                        {
                            pl_sender->sendText(wstring_to_utf8(L"?4[PurePerms] Group " + str2wstr(c_arg[0]) + L" already inherits " + str2wstr(c_arg[1]) + L"."));
                            return CommandResult::ERRORR;
                        }
                    }
                    auto gr = getGroup(c_arg[1]);
                    if (gr.getName() == "")
                    {
                        pl_sender->sendText(wstring_to_utf8(L"?4[PurePerms] Group " + str2wstr(c_arg[0]) + L" does NOT exists."));
                        return CommandResult::ERRORR;
                    }
                    groups[i].addGroup(c_arg[1]);
                    node.reset();
                    for (auto g : groups)
                        node.push_back(g);
                    ::remove("plugins/PurePerms/groups.yml");
                    ofstream fout("plugins/PurePerms/groups.yml");
                    fout << node;
                    fout.close();
                    pl_sender->sendText(wstring_to_utf8(L"?a[PurePerms] Added " + str2wstr(c_arg[1]) + L" to group " + str2wstr(c_arg[0]) + L"'s inheritance list."));
                    return CommandResult::SUCCESS;
                }
            }
            pl_sender->sendText(wstring_to_utf8(L"?4[PurePerms] Group " + str2wstr(c_arg[1]) + L" does NOT exists."));
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
                    for (auto gs : groups[i].getInheritances())
                    {
                        if (gs == c_arg[1])
                        {
                            wcout << L"[PurePerms] Group " + str2wstr(c_arg[0]) + L" already inherits " + str2wstr(c_arg[1]) + L"." << endl;
                            return CommandResult::ERRORR;
                        }
                    }
                    auto gr = getGroup(c_arg[1]);
                    if (gr.getName() == "")
                    {
                        wcout << L"[PurePerms] Group " + str2wstr(c_arg[0]) + L" does NOT exists." << endl;
                        return CommandResult::ERRORR;
                    }
                    groups[i].addGroup(c_arg[1]);
                    node.reset();
                    for (auto g : groups)
                        node.push_back(g);
                    ::remove("plugins/PurePerms/groups.yml");
                    ofstream fout("plugins/PurePerms/groups.yml");
                    fout << node;
                    fout.close();
                    std::cerr << wstring_to_utf8(L"[PurePerms] Added " + str2wstr(c_arg[1]) + L" to group " + str2wstr(c_arg[0]) + L"'s inheritance list.") << endl;
                    return CommandResult::SUCCESS;
                }
            }
            wcout << L"[PurePerms] Group " + str2wstr(c_arg[1]) + L" does NOT exists." << endl;
            return CommandResult::ERRORR;
        }
    }
    else if (provider == "json")
    {
        //??????
        //PS ?????? ? ??????? ? ????????? ????? ? ??????? ? ????? ? ???? ?????? ????
    }
    return CommandResult::FAILED_PERMISSION;
}