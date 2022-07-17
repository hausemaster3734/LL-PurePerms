#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <LoggerAPI.h>
#include <yaml-cpp/yaml.h>
#include <third-party/Nlohmann/json.hpp>
#include "Core.h"
using namespace std;

template<typename...Arg>
struct Prc
{
	Prc() : mems(std::make_tuple(Arg()...)) {}

	std::tuple<Arg...> mems;

};

class PPGroup;

PPGroup getGroup(string name);

class PPGroup
{
public:
	string name;
	string alias;
	vector<string> inheritance;
	vector<string> permissions;
	int isDefault;
	PPGroup() = default;
	PPGroup(string nm, string ali, bool def, vector<string> inh, vector<string> perms)
	{
		name = nm;
		alias = ali;
		isDefault = def;
		inheritance = inh;
		permissions = perms;
	}
	string getName();
	void setName(string _name);
	string getAlias();
	void setAlias(string _alias);
	int IsDefault();
	void setDefault(int b);
	void addGroup(string group);
	void removeGroup(string name);
	void addPermission(string permission);
	void removePermission(string permission);
	vector<string> getInheritances();
	vector<string> getPermissions();
	vector<string> getAllPermissions();
};

namespace YAML {
	template<>
	struct convert<PPGroup> {
		static Node encode(const PPGroup& rhs);
		static bool decode(const Node& node, PPGroup& rhs);
	};
}