#include "PPRELOAD.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

PPReload::PPReload(string command_name) : ICommand(std::move(command_name), "	Reloads all PurePerms configurations.", "/ppreload", { "preload" }) {}

vector<AvailableCommandsPacket::OverloadData> PPReload::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder._break();
    return builder.payload();
}

CommandResult PPReload::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.ppreload") || checkPermission(pl_sender->getName(), "*")))
    {
        YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
        provider = config["provider"].as<string>();
        superadmin_ranks = config["superadmin-ranks"].as<vector<string>>();
        disable_op = config["disable-op"].as<bool>();
        pl_sender->sendText(wstring_to_utf8(L"§2[PurePerms] All plugin configurations have been successfully reloaded."));
        return CommandResult::SUCCESS;
    }
    else if (!pl_sender->isPlayer())
    {
        YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
        provider = config["provider"].as<string>();
        superadmin_ranks = config["superadmin-ranks"].as<vector<string>>();
        disable_op = config["disable-op"].as<bool>();
        cout << "[PurePerms] All plugin configurations have been successfully reloaded.\n";
        return CommandResult::SUCCESS;
    }
    return CommandResult::FAILED_PERMISSION;
}