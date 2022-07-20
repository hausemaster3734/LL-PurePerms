#include "PPInfo.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

PPInfo::PPInfo(string command_name) : ICommand(std::move(command_name), "Shows info about PurePerms.", "/ppinfo", { "pinfo" }) {}

vector<AvailableCommandsPacket::OverloadData> PPInfo::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder._break();
    return builder.payload();
}


CommandResult PPInfo::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.ppinfo") || checkPermission(pl_sender->getName(), "*")))
    {
        pl_sender->sendText(wstring_to_utf8(L"§a[PurePerms] This server is using PurePerms v1.0.0 by TheMrEnderBro(MCBEPluginModer) & wellhause & KocTu4eK"));
        return CommandResult::SUCCESS;
    }
    else if (!pl_sender->isPlayer())
    {
        cout << "[PurePerms] You are currently using PurePerms v1.0.0 by TheMrEnderBro(MCBEPluginModer) & wellhause & KocTu4eK\n";
        return CommandResult::SUCCESS;
    }
    return CommandResult::FAILED_PERMISSION;
}