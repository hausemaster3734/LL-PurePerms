#pragma once
#include "pch.h"

#include <utility>

#include "command/OverloadBuilder.hpp"
#include "command/ICommand.hpp"

class Setgperm : public ICommand {
public:
    explicit Setgperm(string command_name);
    CommandResult execute(Player* pl_sender, string alias_used, vector<string> c_arg) override;
    vector<AvailableCommandsPacket::OverloadData> getOverloads() override;
};
