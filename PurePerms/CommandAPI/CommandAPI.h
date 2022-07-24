//
// Created by pepef on 13.07.2022.
//

#ifndef TEMPLATEPLUGIN_COMMANDAPI_H
#define TEMPLATEPLUGIN_COMMANDAPI_H

#include "../pch.h"

#include <LoggerAPI.h>

#include "CommandManager.h"
#include "../AddGroup.h"
#include "../AddParent.h"
#include "../Defgroup.h"
#include "../FPerms.h"
#include "../Groups.h"
#include "../Grpinfo.h"
#include "../Listgperms.h"
#include "../Listuperms.h"
#include "../PPInfo.h"
#include "../PPReload.h"
#include "../Rmgroup.h"
#include "../RmParent.h"
#include "../PPSudo.h"
#include "../Setgperm.h"
#include "../Setgroup.h"
#include "../Setuperm.h"
#include "../Unsetgperm.h"
#include "../Unsetuperm.h"
#include "../Usrinfo.h"
#include "../EventListener.h"

class CommandAPI {
public:
    CommandAPI() = default;
    void onEnable();
    static void* getCommandManager();
    static Logger getLogger() {
        return Logger("Server");
    }
    static void setTemp(string t);
    static string getTemp();
};

#endif //TEMPLATEPLUGIN_COMMANDAPI_H