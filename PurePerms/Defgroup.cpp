#include "Defgroup.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

Defgroup::Defgroup(string command_name) : ICommand(std::move(command_name), "Allows you to set default group.", "/defgroup <group>", { "defgr" }) {}

vector<AvailableCommandsPacket::OverloadData> Defgroup::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("group");
    builder._break();
    return builder.payload();
}

CommandResult Defgroup::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
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
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.defgroup") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty())
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            for (int i = 0; i < groups.size(); ++i)
            {
                if (groups[i].IsDefault() == 1)
                {
                    groups[i].setDefault(0);
                    for (int j = 0; j < groups.size(); ++j)
                    {
                        if (groups[j].getName() == c_arg[0])
                        {
                            groups[j].setDefault(1);
                        }
                    }
                    node.reset();
                    for (auto g : groups)
                        node.push_back(g);
                    remove("plugins/PurePerms/groups.yml");
                    ofstream fout("plugins/PurePerms/groups.yml");
                    fout << node;
                    fout.close();
                }
                pl_sender->sendText(wstring_to_utf8(L"§2[PurePerms] Default group has been succefully set to " + str2wstr(c_arg[0]) + L"."));
                return CommandResult::SUCCESS;
            }
            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Group " + str2wstr(c_arg[1]) + L" does NOT exist."));
            return CommandResult::ERRORR;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty())
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            for (int i = 0; i < groups.size(); ++i)
            {
                if (groups[i].IsDefault() == 1)
                {
                    groups[i].setDefault(0);
                    for (int j = 0; j < groups.size(); ++j)
                    {
                        if (groups[j].getName() == c_arg[0])
                        {
                            groups[j].setDefault(1);
                        }
                    }
                    node.reset();
                    for (auto g : groups)
                        node.push_back(g);
                    remove("plugins/PurePerms/groups.yml");
                    ofstream fout("plugins/PurePerms/groups.yml");
                    fout << node;
                    fout.close();
                }
                cerr << wstring_to_utf8(L"[PurePerms] Default group has been succefully set to " + str2wstr(c_arg[0]) + L".") << endl;
                return CommandResult::SUCCESS;
            }
            wcout << L"[PurePerms] Group " + str2wstr(c_arg[1]) + L" does NOT exist." << endl;
            return CommandResult::ERRORR;
        }
    }
    else if (provider == "json")
    {
        //TODO ΐλεκρε
    }
    return CommandResult::FAILED_PERMISSION;
}