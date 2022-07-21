#include "pch.h"
#include "Setuperm.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

Setuperm::Setuperm(string command_name) : ICommand(std::move(command_name), "Adds a permission to the user.", "/setuperm <user> <permission>", { "superm" }) {}

vector<AvailableCommandsPacket::OverloadData> Setuperm::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("user");
    builder.addData("permission");
    builder._break();
    return builder.payload();
}

CommandResult Setuperm::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
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
            if (c_arg.empty() || c_arg.size() == 1)
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            auto player = getPlayer(c_arg[0]);
            if (player.getNickname() == "")
            {
                pl_sender->sendText(wstring_to_utf8(L"�4[PurePerms] Player " + str2wstr(c_arg[0]) + L" does NOT exist."));
                return CommandResult::ERRORR;
            }
            player.addPermission(c_arg[1]);
            YAML::Node pl;
            pl = player;
            string path("plugins/PurePerms/players/" + c_arg[0] + ".yml");
            remove(path.c_str());
            ofstream fout(path);
            fout << pl;
            fout.close();
            pl_sender->sendText(wstring_to_utf8(L"�a[PurePerms] Added permission " + str2wstr(c_arg[1]) + L" to " + str2wstr(c_arg[0]) + L" successfully."));
            return CommandResult::SUCCESS;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty() || c_arg.size() == 1)
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            auto player = getPlayer(c_arg[0]);
            if (player.getNickname() == "")
            {
                wcout << L"[PurePerms] Player " + str2wstr(c_arg[0]) + L" does NOT exist.\n";
                return CommandResult::ERRORR;
            }
            player.addPermission(c_arg[1]);
            YAML::Node pl;
            pl = player;
            string path("plugins/PurePerms/players/" + c_arg[0] + ".yml");
            remove(path.c_str());
            ofstream fout(path);
            fout << pl;
            fout.close();
            cout << "[PurePerms] Added permission " + c_arg[1] + " to " + c_arg[0] + " successfully.\n";
            return CommandResult::SUCCESS;
        }
        return CommandResult::FAILED_PERMISSION;
    }
    else if (provider == "json")
    {
        //������
        //PS ������ � ������� � ��������� ����� � ������� � ����� � ���� ������ ����
    }
}