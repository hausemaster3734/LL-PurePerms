#include "pch.h"
#include "FPerms.h"
#include "PPGroup.h"
#include <MC/Actor.hpp>
#include <Utils/StringHelper.h>
#include <LLAPI.h>

map<string, int> fperms_counter;

Fperms::Fperms(string command_name) : ICommand(std::move(command_name), "Allows you to find permissions for a specific plugin.", "/fperms <plugin>", { "fp" }) {}

vector<AvailableCommandsPacket::OverloadData> Fperms::getOverloads()
{
    OverloadBuilder builder = OverloadBuilder();
    builder.addData("plugin");
    builder._break();
    return builder.payload();
}

CommandResult Fperms::execute(Player* pl_sender, string alias_used, vector<string> c_arg)
{
    int counter = 0, cmd_counter = 0;
    if (provider == "yaml")
    {
        if (pl_sender->isPlayer() && (checkPermission(pl_sender->getName(), "pperms.command.fperms") || checkPermission(pl_sender->getName(), "*")))
        {
            if (c_arg.empty())
            {
                pl_sender->sendText("Alias used: " + alias_used);
                return CommandResult::INVALID_ARGS;
            }
            vector<wstring> perms;
            ifstream fin(std::filesystem::temp_directory_path().string() + "/" + c_arg[0] + ".tmp");
            if (!fin.is_open())
            {
                fin.close();
                pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Plugin " + str2wstr(c_arg[0]) + L" not exist."));
                return CommandResult::ERRORR;
            }
            else
            {
                string tmp;
                while (getline(fin, tmp))
                {
                    perms.push_back(str2wstr(tmp));
                }
                fin.close();
            }
            if (perms.size() == 0)
            {
                pl_sender->sendText(wstring_to_utf8(L"§4[PurePerms] Permissions not found."));
                return CommandResult::ERRORR;
            }
            double size = double(double(perms.size()) / 5.0);
            double fr = size - long(size);
            static vector<vector<wstring>> output_messages;
            if (perms.size() > 5 && fr > 0.0 && fr < 1.0)
            {
                if (fperms_counter[pl_sender->getName()] > size || fperms_counter[pl_sender->getName()] == 0)
                    fperms_counter[pl_sender->getName()] = 0;
                int id = 0;
                for (int i = 1; i < size; ++i)
                {
                    vector<wstring> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(perms[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                for (int i = 0; i < 1; ++i)
                {
                    auto beg = perms.begin() + id;
                    auto end = perms.end();
                    vector<wstring> tmp(beg, end);
                    output_messages.push_back(tmp);
                }
                wstring out = L"§a[PurePerms] List of all plugin permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(fperms_counter[pl_sender->getName()] + 1)) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[fperms_counter[pl_sender->getName()]])
                    out += L"§a[PurePerms] - " + c + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                fperms_counter[pl_sender->getName()] += 1;
            }
            else if (perms.size() >= 5 && fr == 0)
            {
                if (fperms_counter[pl_sender->getName()] > size)
                    fperms_counter[pl_sender->getName()] = 0;
                int id = 0;
                for (int i = 0; i < size; ++i)
                {
                    vector<wstring> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(perms[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                wstring out = L"§a[PurePerms] List of all plugin permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(fperms_counter[pl_sender->getName()] + 1)) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[fperms_counter[pl_sender->getName()]])
                    out += L"§a[PurePerms] - " + c + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                fperms_counter[pl_sender->getName()] += 1;
            }
            else if (perms.size() > 0 && perms.size() < 5)
            {
                if (fperms_counter[pl_sender->getName()] > 1)
                    fperms_counter[pl_sender->getName()] = 0;
                vector<wstring> tmp;
                for (int i = 0; i < size; ++i)
                {
                   tmp.push_back(perms[i]);
                }
                output_messages.push_back(tmp);
                wstring out = L"§a[PurePerms] List of all plugin permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(fperms_counter[pl_sender->getName()]+1)) + L" / " + utf8_to_wstring(to_string((int)size)) + L"):\n";
                for (auto c : output_messages[fperms_counter[pl_sender->getName()]-1])
                    out += L"§a[PurePerms] - " + c + L"\n";
                pl_sender->sendText(wstring_to_utf8(out));
                fperms_counter[pl_sender->getName()] += 1;
            }
            return CommandResult::SUCCESS;
        }
        else if (!pl_sender->isPlayer())
        {
            if (c_arg.empty())
            {
                cout << "Alias used: " + alias_used << endl;
                return CommandResult::INVALID_ARGS;
            }
            vector<wstring> perms;
            ifstream fin(std::filesystem::temp_directory_path().string() + "/" + c_arg[0] + ".tmp");
            if (!fin.is_open())
            {
                fin.close();
                cout << wstring_to_utf8(L"[PurePerms] Plugin " + str2wstr(c_arg[0]) + L" does NOT exist.\n");
                return CommandResult::ERRORR;
            }
            else
            {
                string tmp;
                while (getline(fin, tmp))
                {
                    perms.push_back(str2wstr(tmp));
                }
                fin.close();
            }
            if (perms.size() == 0)
            {
                cout << wstring_to_utf8(L"[PurePerms] Permissions not found.\n");
                return CommandResult::ERRORR;
            }
            double size = double(double(perms.size()) / 5.0);
            double fr = size - long(size);
            static vector<vector<wstring>> output_messages;
            if (perms.size() > 5 && fr > 0.0 && fr < 1.0)
            {
                if (fperms_counter["Console"] > size)
                    fperms_counter["Console"] = 0;
                int id;
                for (int i = 1; i < size; ++i)
                {
                    vector<wstring> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        tmp.push_back(perms[id]);
                        id++;
                    }
                    output_messages.push_back(tmp);
                }
                for (int i = 0; i < 1; ++i)
                {
                    auto beg = perms.begin() + id;
                    auto end = perms.end();
                    vector<wstring> tmp(beg, end);
                    output_messages.push_back(tmp);
                }
                wstring out = L"[PurePerms] List of all plugin permissions from " + str2wstr(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(fperms_counter["Console"] + 1)) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[fperms_counter["Console"]])
                    out += L"[PurePerms] - " + c + L"\n";
                std::wcout << out << endl;
                fperms_counter["Console"] += 1;
            }
            else if (perms.size() >= 5 && fr == 0)
            {
                if (fperms_counter["Console"] > size)
                    fperms_counter["Console"] = 0;
                int id;
                for (int i = 1; i < size; ++i)
                {
                    vector<wstring> tmp;
                    for (int j = 0; j < 5; ++j)
                    {
                        id++;
                        tmp.push_back(perms[id]);
                    }
                    output_messages.push_back(tmp);
                }
                wstring out = L"[PurePerms] List of all plugin permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(fperms_counter["Console"] + 1)) + L" / " + utf8_to_wstring(to_string((int)size + 1)) + L"):\n";
                for (auto c : output_messages[fperms_counter["Console"]])
                    out += L"[PurePerms] - " + c + L"\n";
                std::wcout << out << endl;
                fperms_counter["Console"] += 1;
            }
            else if (perms.size() > 0 && perms.size() < 5)
            {
                if (fperms_counter["Console"] > 1)
                    fperms_counter["Console"] = 0;
                vector<wstring> tmp;
                for (int i = 0; i < size; ++i)
                {
                    tmp.push_back(perms[i]);
                }
                output_messages.push_back(tmp);
                wstring out = L"[PurePerms] List of all plugin permissions from " + utf8_to_wstring(c_arg[0]);
                out += L"(" + utf8_to_wstring(to_string(fperms_counter[pl_sender->getName()]+1)) + L" / " + utf8_to_wstring(to_string((int)size)) + L"):\n";
                for (auto c : output_messages[fperms_counter[pl_sender->getName()]-1])
                    out += L"[PurePerms] - " + c + L"\n";
                std::wcout << out << endl;
                fperms_counter["Console"] += 1;
            }
            return CommandResult::SUCCESS;
        }
    }
    else if (provider == "json")
    {

    }
    return CommandResult::FAILED_PERMISSION;
}