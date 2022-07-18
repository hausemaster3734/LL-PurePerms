//
// Created by pepef on 13.07.2022.
//

#include "CommandAPI.h"

static void* command_manager;
static string temp;

void CommandAPI::onEnable() {
    command_manager = new CommandManager();
    CommandManager* local = ((CommandManager *) command_manager);
    local->addCommand(new AddGroup("addgroup"));
    local->addCommand(new AddParent("addparent"));
    local->addCommand(new Defgroup("defgroup"));
    local->addCommand(new Fperms("fperms"));
    local->addCommand(new Groups("groups"));
    local->addCommand(new Grpinfo("grpinfo"));
    local->addCommand(new Listgperms("listgperms"));
    local->addCommand(new Listuperms("listuperms"));
    local->addCommand(new PPInfo("ppinfo"));
    local->addCommand(new PPReload("ppreload"));
    local->addCommand(new RmGroup("rmgroup"));
    local->addCommand(new RmParent("rmparent"));
    local->addCommand(new PPSudo("ppsudo"));
    local->addCommand(new Setgperm("setgperm"));
    local->addCommand(new Setgroup("setgroup"));
    local->addCommand(new Setuperm("setuperm"));
    local->addCommand(new Unsetgperm("unsetgperm"));
    local->addCommand(new Unsetuperm("unsetuperm"));
    local->addCommand(new Usrinfo("usrinfo"));
    local->init();
    EventListener *listener = new EventListener();
    listener->init();
}

void* CommandAPI::getCommandManager() {
    return command_manager;
}

void CommandAPI::setTemp(string t) {
    temp = std::move(t);
}

string CommandAPI::getTemp() {
    return temp;
}