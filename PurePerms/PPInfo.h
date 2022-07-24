#pragma once
#include "pch.h"

#include <utility>

#include "CommandAPI/OverloadBuilder.hpp"
#include "CommandAPI/ICommand.hpp"

class PPInfo : public ICommand {
public:
    explicit PPInfo(string command_name);
    CommandResult execute(Player* pl_sender, string alias_used, vector<string> c_arg) override;
    vector<AvailableCommandsPacket::OverloadData> getOverloads() override;
};
