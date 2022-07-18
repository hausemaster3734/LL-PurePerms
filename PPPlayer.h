#pragma once
#include "PPGroup.h"

class PPPlayer;

PPPlayer getPlayer(string nickname);

class PPPlayer
{
	string nickname;
	string group;
	string noeulPW;
	vector<string> permissions;
	int time;
	int expTime;
public:
	PPPlayer() = default;
	PPPlayer(string nick, string gr, vector<string> perms, string pw,int tm, int exptm)
	{
		nickname = nick;
		group = gr;
		noeulPW = pw;
		permissions = perms;
		time = tm;
		expTime = exptm;
	}
	string getNickname();
	void setNickname(string);
	string getPassword() { return noeulPW; }
	void setPassword(string);
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