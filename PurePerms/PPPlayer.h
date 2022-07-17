#pragma once
#include "PPGroup.h"

class PPPlayer;

PPPlayer getPlayer(string nickname);

class PPPlayer
{
	string nickname;
	string group;
	vector<string> permissions;
	int time;
	int expTime;
public:
	PPPlayer() = default;
	PPPlayer(string nick, string gr, vector<string> perms, int tm, int exptm)
	{
		nickname = nick;
		group = gr;
		permissions = perms;
		time = tm;
		expTime = exptm;
	}
	string getNickname();
	void setNickname(string);
	class PPGroup getGroup();
	void setGroup(string);
	vector<string> getPermissions();
	void addPermission(string);
	void removePermission(string);
	int getTime();
	void setTime(int);
	int getExpTime();
	void setExpTime(int);
};

namespace YAML {
	template<>
	struct convert<PPPlayer> {
		static Node encode(const PPPlayer& rhs);
		static bool decode(const Node& node, PPPlayer& rhs);
	};
}