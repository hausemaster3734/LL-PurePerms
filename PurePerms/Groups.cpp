#include "Groups.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

Groups::Groups(string command_name) : ICommand(std::move(command_name), "Shows a list of all groups.", "/groups", { "grps" }) {}

vector<AvailableCommandsPacket::OverloadData> Groups::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder._break();
    return builder.payload();
}

CommandResult Groups::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
  //  YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
    if (provider == "yaml")
    {
        vector<PPGroup> groups;
        YAML::Node node = YAML::LoadFile("plugins/PurePerms/groups.yml");
        for (auto gr : node)
        {
            groups.push_back(gr.as<PPGroup>());
        }
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.groups") || checkPermission(pl_sender->getName(), "*")))
        {
            wstring out = L"§2[PurePerms] All registered groups: ";
            for (auto c : groups)
            {
                out += str2wstr(c.getName()) + L", ";
            }
            wstring output(out.begin(), out.end() - 2);
            pl_sender->sendText(wstring_to_utf8(output));
            return CommandResult::SUCCESS;
        }
        else if (!pl_sender->isPlayer())
        {
            wstring out = L"[PurePerms] All registered groups: ";
            for (auto c : groups)
            {
                out += str2wstr(c.getName()) + L", ";
            }
            wstring output(out.begin(), out.end() - 2);
            wcout << output << endl;
            return CommandResult::SUCCESS;
        }
    }
    else if (provider == "json")
    {
        //TODO Алексе
    }
    return CommandResult::FAILED_PERMISSION;
}