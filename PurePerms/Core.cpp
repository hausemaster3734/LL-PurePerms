#include "pch.h"
#include "Core.h"
#include <codecvt>

string implode(const string& glue, const vector<string>& pieces)
{
    string a;
    int leng = pieces.size();
    for (int i = 0; i < leng; i++)
    {
        a += pieces[i];
        if (i < (leng - 1))
            a += glue;
    }
    return a;
}

vector<string> split(string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::string wstring_to_utf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

std::wstring utf8_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

bool checkPermission(string nick,string perm)
{
    if (provider == "yaml")
    {
        auto pl = getPlayer(nick);
        for (auto g : pl.getPermissions())
        {
            if (g == perm)
            {
                return 1;
            }
        }
        auto gr = pl.getGroup();
        for (auto g : gr.getAllPermissions())
        {
            if (g == perm)
            {
                return 1;
            }
        }
        return 0;
    }
    else if (provider == "json")
    {

    }
}