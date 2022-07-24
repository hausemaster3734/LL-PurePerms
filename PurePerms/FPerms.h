#pragma once
#include "pch.h"

#include <utility>

#include "CommandAPI/OverloadBuilder.hpp"
#include "CommandAPI/ICommand.hpp"

class Fperms : public ICommand {
public:
    explicit Fperms(string command_name);
    CommandResult execute(Player* pl_sender, string alias_used, vector<string> c_arg) override;
    vector<AvailableCommandsPacket::OverloadData> getOverloads() override;
};
