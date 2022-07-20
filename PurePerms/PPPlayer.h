#pragma once
#include "PPGroup.h"
using namespace std;

class PPPlayer
{
	string nickname;
	string group;
	string prefix;
	string suffix;
	string noeulPW;
	vector<string> permissions;
	int time;
	int expTime;
public:
	PPPlayer() = default;
	PPPlayer(string nick, string gr, vector<string> perms, string pw, string prefix1, string suffix1, int tm, int exptm)
	{
		nickname = nick;
		group = gr;
		prefix = prefix1;
		suffix = suffix1;
		noeulPW = pw;
		permissions = perms;
		time = tm;
		expTime = exptm;
	}
	string getNickname();
	void setNickname(string);
	string getPassword() { return noeulPW; }
	void setPassword(string);
	string getPrefix() { return prefix; }
	void setPrefix(string);
	string getSuffix() { return suffix; }
	void setSuffix(string);
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

PPPlayer getPlayer(string nickname);

namespace YAML {
	template<>
	struct convert<PPPlayer> {
		static Node encode(const PPPlayer& rhs);
		static bool decode(const Node& node, PPPlayer& rhs);
	};
}