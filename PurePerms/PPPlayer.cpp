#include "pch.h"
#include "PPPlayer.h"
#include "Core.h"

vector<PPPlayer*> players;

string PPPlayer::getNickname()
{
	return nickname;
}

void PPPlayer::setNickname(string nick)
{
	nickname = nick;
}

void PPPlayer::setPassword(string password)
{
	noeulPW = password;
}

void PPPlayer::setPrefix(string pref)
{
	prefix = pref;
}

void PPPlayer::setSuffix(string suff)
{
	suffix = suff;
}

PPGroup PPPlayer::getGroup()
{
	return ::getGroup(group);
}

void PPPlayer::setGroup(string gr)
{
	group = gr;
}

vector<string> PPPlayer::getPermissions()
{
	return permissions;
}

void PPPlayer::addPermission(string perm)
{
	permissions.push_back(perm);
}

void PPPlayer::removePermission(string permission)
{
	vector<string> result;
	for (auto perm : permissions)
	{
		if (perm == permission)
		{
			continue;
		}
		result.push_back(perm);
	}
	permissions = result;
}

int PPPlayer::getTime()
{
	return time;
}

void PPPlayer::setTime(int tm)
{
	time = tm;
}

int PPPlayer::getExpTime()
{
	return expTime;
}

void PPPlayer::setExpTime(int tm)
{
	expTime = tm;
}


YAML::Node YAML::convert<PPPlayer>::encode(const PPPlayer& rhs)
{
	Node node;
	node["userName"] = ((PPPlayer)rhs).getNickname();
	node["group"] = ((PPPlayer)rhs).getGroup().getName();
	node["prefix"] = ((PPPlayer)rhs).getPrefix();
	node["suffix"] = ((PPPlayer)rhs).getSuffix();
	node["noeulPW"] = ((PPPlayer)rhs).getPassword();
	if (((PPPlayer)rhs).getPermissions().size() == 0)
		node["permissions"] = vector<string>();
	else
	{
		for (auto inh : ((PPPlayer)rhs).getPermissions())
		{
			node["permissions"].push_back(inh);
		}
	}
	node["time"] = ((PPPlayer)rhs).getTime();
	node["expTime"] = ((PPPlayer)rhs).getExpTime();
	return node;
}

bool YAML::convert<PPPlayer>::decode(const Node& node, PPPlayer& rhs)
{
	rhs.setNickname(node["userName"].as<string>());
	rhs.setGroup(node["group"].as<string>());
	rhs.setPrefix(node["prefix"].as<string>());
	rhs.setSuffix(node["suffix"].as<string>());
	rhs.setPassword(node["noeulPW"].as<string>());
	for (auto v : node["permissions"].as<vector<string>>())
		rhs.addPermission(v);
	rhs.setTime(node["time"].as<int>());
	rhs.setExpTime(node["expTime"].as<int>());
	return true;
}

PPPlayer getPlayer(string name)
{
	PPPlayer pp;
	YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
	if (config["provider"].as<string>() == "yaml")
	{
		ifstream test("plugins/PurePerms/players/" + name + ".yml");
		if (!test.is_open())
		{
			for (auto g : players)
			{
				if (g->getNickname() == name)
				{
					pp = *g;
					return pp;
				}
			}
		}
		else
		{
			YAML::Node groups = YAML::LoadFile("plugins/PurePerms/players/" + name + ".yml");
			auto gro = groups.as<PPPlayer>();
			if (gro.getNickname() == name)
			{
				return gro;
			}
		}
		return pp;
	}
	else if (config["provider"].as<string>() == "json")
	{
		//TODO: Алексе здесь уже твой код будет
	}
}