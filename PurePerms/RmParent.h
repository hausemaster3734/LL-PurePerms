#pragma once
#include "pch.h"

#include <utility>

#include "CommandAPI/OverloadBuilder.hpp"
#include "CommandAPI/ICommand.hpp"

class RmParent : public ICommand {
public:
    explicit RmParent(string command_name);
    CommandResult execute(Player* pl_sender, string alias_used, vector<string> c_arg) override;
    vector<AvailableCommandsPacket::OverloadData> getOverloads() override;
};
