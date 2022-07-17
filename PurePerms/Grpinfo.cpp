#include "Grpinfo.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

Grpinfo::Grpinfo(string command_name) : ICommand(std::move(command_name), "Shows info about a group.", "/grpinfo <group>", { "ginfo" }) {}

vector<AvailableCommandsPacket::OverloadData> Grpinfo::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("group");
    builder._break();
    return builder.payload();
}

CommandResult Grpinfo::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
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
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.grpinfo") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty())
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            for (auto g : groups)
            {
                if (g.name == c_arg[0])
                {
                    wstring out = L"§2[PurePerms] -- Group Information for " + str2wstr(c_arg[0]) + L" --\n";
                    out += L"ALIAS: " + str2wstr(g.alias) + L"\n";
                    out += L"DEFAULT: " + str2wstr(to_string(g.isDefault)) + L"\n";
                    out += L"PARENTS: ";
                    int cnt = 0;
                    for (auto gs : g.getInheritances())
                    {
                        if (cnt == 0)
                        {
                            out += str2wstr(gs) + L"\n";
                        }
                        else
                        {
                            out += L"§2[PurePerms] " + str2wstr(gs) + L"\n";
                        }
                        cnt++;
                    }
                    wstring output(out.begin(), out.end() - 1);
                    pl_sender->sendText(wstring_to_utf8(output));
                    return CommandResult::SUCCESS;
                }
            }
            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Group " + str2wstr(c_arg[0]) + L" does NOT exist."));
            return CommandResult::ERRORR;
        }
        else if (!pl_sender->isPlayer())
        {

            if (c_arg.empty())
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            for (auto g : groups)
            {
                if (g.name == c_arg[0])
                {
                    wstring out = L"[PurePerms] -- Group Information for " + str2wstr(c_arg[0]) + L" --\n";
                    out += L"ALIAS: " + str2wstr(g.alias) + L"\n";
                    out += L"DEFAULT: " + str2wstr(to_string(g.isDefault)) + L"\n";
                    out += L"PARENTS: ";
                    int cnt = 0;
                    for (auto gs : g.getInheritances())
                    {
                        if (cnt == 0)
                        {
                            out += str2wstr(gs) + L"\n";
                        }
                        else
                        {
                            out += L"[PurePerms] " + str2wstr(gs) + L"\n";
                        }
                        cnt++;
                    }
                    wstring output(out.begin(), out.end() - 1);
                    std::wcout << output << endl;
                    return CommandResult::SUCCESS;
                }
            }
            std::wcout << L"[PurePerms] Group " + str2wstr(c_arg[0]) + L" does NOT exist." << endl;
            return CommandResult::ERRORR;
        }
    }
    else if (provider == "json")
    {
        //TODO ΐλεκρε
    }
    return CommandResult::FAILED_PERMISSION;
}