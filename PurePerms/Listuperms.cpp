#include "pch.h"
#include "Listuperms.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

Listuperms::Listuperms(string command_name) : ICommand(std::move(command_name), "Shows a list of all permissions from a user.", "/listuperms <player> <page>", { "lup" }) {}

vector<AvailableCommandsPacket::OverloadData> Listuperms::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("player");
    builder._break();
    return builder.payload();
}


CommandResult Listuperms::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
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
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.listuperms") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty() || c_arg.size() == 1)
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            static vector<vector<string>> output_messages;
            auto usr = getPlayer(c_arg[0]);
            if (usr.getNickname() == "")
            {
                pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Player " + str2wstr(c_arg[0]) + L" not found!"));
                return CommandResult::ERRORR;
            }
            double size = double(double(usr.getPermissions().size()) / 5.0);
            double fr = size - long(size);
            if (usr.getPermissions().size() >= 5 && fr > 0.0 && fr < 1.0)
            {
                int id = 0;
                for (int i = 1; i < size; ++i)
                {
                    vector<string> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(usr.getPermissions()[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                for (int i = 0; i < 1; ++i)
                {
                    auto beg = usr.getPermissions().begin() + id;
                    auto end = usr.getPermissions().end();
                    vector<string> tmp(beg, end);
                    output_messages.push_back(tmp);
                };
                if (atoi(c_arg[1].c_str()) > output_messages.size())
                {
                    pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Maximum pages = " + str2wstr(to_string(output_messages.size())) + L"."));
                    return CommandResult::ERRORR;
                }
                wstring out = L"§a[PurePerms] List of all user permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()) + 1)) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str())])
                    out += L"§a[PurePerms] - " + str2wstr(c) + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                return CommandResult::SUCCESS;
            }
            else if (usr.getPermissions().size() >= 5 && fr == 0)
            {
                int id = 0;
                for (int i = 1; i < size; ++i)
                {
                    vector<string> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(usr.getPermissions()[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                if (atoi(c_arg[1].c_str()) > output_messages.size())
                {
                    pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Maximum pages = " + str2wstr(to_string(output_messages.size())) + L"."));
                    return CommandResult::ERRORR;
                }
                wstring out = L"§a[PurePerms] List of all user permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()) + 1)) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str()) - 1])
                    out += L"§a[PurePerms] - " + str2wstr(c) + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                return CommandResult::SUCCESS;
            }
            else if (usr.getPermissions().size() > 0 && usr.getPermissions().size() < 4)
            {
                if (atoi(c_arg[1].c_str()) > 1)
                {
                    pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Maximum pages = 1."));
                    return CommandResult::ERRORR;
                }
                vector<string> tmp;
                for (int i = 0; i < usr.getPermissions().size(); ++i)
                {
                    tmp.push_back(usr.getPermissions()[i]);
                }
                output_messages.push_back(tmp);
                wstring out = L"§a[PurePerms] List of all user permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()))) + L" / 1):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str()) - 1])
                    out += L"§a[PurePerms] - " + str2wstr(c) + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                return CommandResult::SUCCESS;
            }
            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Player " + str2wstr(c_arg[0]) + L" doesn't have any user permissions."));
            return CommandResult::ERRORR;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty() || c_arg.size() == 1)
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            static vector<vector<string>> output_messages;
            auto usr = getPlayer(c_arg[0]);
            if (usr.getNickname() == "")
            {
               std::wcout << L"[PurePerms] Player " + str2wstr(c_arg[0]) + L" not found!" << endl;
               return CommandResult::ERRORR;
            }
            double size = double(double(usr.getPermissions().size()) / 5.0);
            double fr = size - long(size);
            if (usr.getPermissions().size() >= 5 && fr > 0.0 && fr < 1.0)
            {
                int id = 0;
                for (int i = 1; i < size; ++i)
                {
                    vector<string> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(usr.getPermissions()[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                for (int i = 0; i < 1; ++i)
                {
                    auto beg = usr.getPermissions().begin() + id;
                    auto end = usr.getPermissions().end();
                    vector<string> tmp(beg, end);
                    output_messages.push_back(tmp);
                };
                if (atoi(c_arg[1].c_str()) > output_messages.size())
                {
                    cout << "[PurePerms]: Maximum page - " + to_string(usr.getPermissions().size()) + "." << endl;
                    return CommandResult::ERRORR;
                }
                wstring out = L"[PurePerms] List of all user permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()) + 1)) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str())])
                    out += L"[PurePerms] - " + str2wstr(c) + L"\n";
                std::wcout << out << endl;
                return CommandResult::SUCCESS;
            }
            else if (usr.getPermissions().size() >= 5 && fr == 0)
            {
                int id = 0;
                for (int i = 1; i < size; ++i)
                {
                    vector<string> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(usr.getPermissions()[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                if (atoi(c_arg[1].c_str()) > output_messages.size())
                {
                    cout << "[PurePerms]: Maximum page - " + to_string(usr.getPermissions().size()) + "." << endl;
                    return CommandResult::ERRORR;
                }
                wstring out = L"[PurePerms] List of all user permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()) + 1)) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str()) - 1])
                    out += L"[PurePerms] - " + str2wstr(c) + L"\n";
                std::wcout << out << endl;
                return CommandResult::SUCCESS;
            }
            else if (usr.getPermissions().size() > 0 && usr.getPermissions().size() < 5)
            {
                if (atoi(c_arg[1].c_str()) > 1)
                {
                    std::wcout << L"[PurePerms] Maximum pages = 1." << endl;
                    return CommandResult::ERRORR;
                }
                vector<string> tmp;
                for (int i = 0; i < usr.getPermissions().size(); ++i)
                {
                    tmp.push_back(usr.getPermissions()[i]);
                }
                output_messages.push_back(tmp);
                wstring out = L"[PurePerms] List of all user permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()))) + L" / 1):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str()) - 1])
                    out += L"[PurePerms] - " + str2wstr(c) + L"\n";
                std::wcout << out << endl;
                return CommandResult::SUCCESS;
            }
            std::wcout << L"[PurePerms]  Player " + str2wstr(c_arg[0]) + L" doesn't have any user permissions." << endl;
            return CommandResult::ERRORR;
        }
    }
    else if (provider == "json")
    {
        //TODO Àëåêñå
    }
    return CommandResult::FAILED_PERMISSION;
}