#include "Listgperms.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>

Listgperms::Listgperms(string command_name) : ICommand(std::move(command_name), "Shows a list of all permissions from a group.", "/listgperms <group> <page>", { "lgp" }) {}

vector<AvailableCommandsPacket::OverloadData> Listgperms::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("group");
    builder._break();
    return builder.payload();
}


CommandResult Listgperms::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
   // YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
    if (provider == "yaml")
    {
        vector<PPGroup> groups;
        YAML::Node node = YAML::LoadFile("plugins/PurePerms/groups.yml");
        for (auto gr : node)
        {
            groups.push_back(gr.as<PPGroup>());
        }
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.listgperms") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty() || c_arg.size() == 1)
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            vector<vector<string>> output_messages;
            auto gro = getGroup(c_arg[0]);
            double size = double(double(gro.permissions.size()) / 5.0);
            double fr = size - long(size);
            if (gro.permissions.size() > 5 && fr > 0.0 && fr < 1.0)
            {
                int id = 0;
                for (int i = 1; i < size; ++i)
                {
                    vector<string> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(gro.permissions[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                for (int i = 0; i < 1; ++i)
                {
                    auto beg = gro.permissions.begin() + id;
                    auto end = gro.permissions.end();
                    vector<string> tmp(beg, end);
                    output_messages.push_back(tmp);
                }
                if (atoi(c_arg[1].c_str()) > output_messages.size())
                {
                    pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Maximum pages = " + str2wstr(to_string(output_messages.size())) + L"."));
                    return CommandResult::ERRORR;
                }
                wstring out = L"§a[PurePerms] List of all group permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()))) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str()) - 1])
                    out += L"§a[PurePerms] - " + str2wstr(c) + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                return CommandResult::SUCCESS;
            }
            else if (gro.permissions.size() >= 5 && fr == 0)
            {
                int id = 0;
                for (int i = 0; i < size; ++i)
                {
                    vector<string> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(gro.permissions[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                if (atoi(c_arg[1].c_str()) > output_messages.size())
                {
                    pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Maximum pages = " + str2wstr(to_string(output_messages.size())) + L"."));
                    return CommandResult::ERRORR;
                }
                wstring out = L"§a[PurePerms] List of all group permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()))) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str()) - 1])
                    out += L"§a[PurePerms] - " + str2wstr(c) + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                return CommandResult::SUCCESS;
            }
            else if (gro.permissions.size() > 0 && gro.permissions.size() < 4)
            {
                if (atoi(c_arg[1].c_str()) > 1)
                {
                    pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Maximum pages = 1."));
                    return CommandResult::ERRORR;
                }
                vector<string> tmp;
                for (int i = 0; i < gro.permissions.size(); ++i)
                {
                    tmp.push_back(gro.permissions[i]);
                }
                output_messages.push_back(tmp);
                wstring out = L"§a[PurePerms] List of all group permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()))) + L" / 1):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str())-1])
                    out += L"§a[PurePerms] - " + str2wstr(c) + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                return CommandResult::SUCCESS;
            }
            pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Group " + str2wstr(c_arg[0]) + L" does NOT exist."));
            return CommandResult::ERRORR;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty() || c_arg.size() == 1)
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            vector<vector<string>> output_messages;
            auto gro = getGroup(c_arg[0]);
            double size = double(double(gro.permissions.size()) / 5.0);
            double fr = size - long(size);
            if (gro.permissions.size() >= 5 && fr > 0.0 && fr < 1.0)
            {
                int id = 0;
                for (int i = 1; i < size; ++i)
                {
                    vector<string> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(gro.permissions[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                for (int i = 0; i < 1; ++i)
                {
                    auto beg = gro.permissions.begin() + id;
                    auto end = gro.permissions.end();
                    vector<string> tmp(beg, end);
                    output_messages.push_back(tmp);
                };
                if (atoi(c_arg[1].c_str()) > output_messages.size())
                {
                    std::wcout << L"[PurePerms] Maximum pages = " + str2wstr(to_string(output_messages.size())) + L".\n";
                    return CommandResult::ERRORR;
                }
                wstring out = L"[PurePerms] List of all group permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()))) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str())-1])
                    out += L"[PurePerms] - " + str2wstr(c) + L"\n";
                std::wcout << out << endl;
                return CommandResult::SUCCESS;
            }
            else if (gro.permissions.size() >= 5 && fr == 0)
            {
                int id = 0;
                for (int i = 0; i < size; ++i)
                {
                    vector<string> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(gro.permissions[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                if (atoi(c_arg[1].c_str()) > output_messages.size())
                {
                    std::wcout << L"[PurePerms] Maximum pages = " + str2wstr(to_string(output_messages.size())) + L".\n";
                    return CommandResult::ERRORR;
                }
                wstring out = L"[PurePerms] List of all group permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()))) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str()) - 1])
                    out += L"[PurePerms] - " + str2wstr(c) + L"\n";
                std::wcout << out << endl;
                return CommandResult::SUCCESS;
            }
            else if (gro.permissions.size() > 0 && gro.permissions.size() < 5)
            {
                if (atoi(c_arg[1].c_str()) > 1)
                {
                    std::wcout << L"[PurePerms] Maximum pages = 1." << endl;
                    return CommandResult::ERRORR;
                }
                vector<string> tmp;
                for (int i = 0; i < gro.permissions.size(); ++i)
                {
                    tmp.push_back(gro.permissions[i]);
                }
                output_messages.push_back(tmp);
                wstring out = L"[PurePerms] List of all group permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(atoi(c_arg[1].c_str()))) + L" / 1):\n";
                for (auto c : output_messages[atoi(c_arg[1].c_str())-1])
                    out += L"[PurePerms] - " + str2wstr(c) + L"\n";
                std::wcout << out << endl;
                return CommandResult::SUCCESS;
            }
            std::wcout << L"[PurePerms] Group " + str2wstr(c_arg[0]) + L" does NOT exist." << endl;
            return CommandResult::ERRORR;
        }
    }
    else if (provider == "json")
    {
        //TODO Àëåêñå
    }
    return CommandResult::FAILED_PERMISSION;
}