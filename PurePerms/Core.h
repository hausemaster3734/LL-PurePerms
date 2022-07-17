#pragma once
#include <string>
#include <vector>
#include "PPGroup.h"
#include "PPPlayer.h"
using namespace std;

string implode(const string& glue, const vector<string>& pieces);
template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

vector<string> split(string s, string delimiter);

std::string wstring_to_utf8(const std::wstring& str);

std::wstring utf8_to_wstring(const std::string& str);

class PPGroup;

class PPPlayer;

extern vector<PPGroup*> groups;

extern vector<PPPlayer*> players;

extern string provider;
extern vector<string> superadmin_ranks;
extern bool disable_op;

bool checkPermission(string nick,string perm);