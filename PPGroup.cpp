#include "pch.h"
#include "PPGroup.h"

vector<PPGroup*> groups;

string PPGroup::getName() { return name; }

void PPGroup::setName(string _name)
{
	if (name == "")
	{
		name = _name;
		return;
	}
	string oldName = name;
	name = _name;
	//YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
	if (provider == "yaml")
	{
		ifstream test("plugins/PurePerms/groups.yml");
		if (!test.is_open())
		{
			test.close();
			YAML::Node wr;
			wr.push_back(*this);
			ofstream fout("plugins/PurePerms/groups.yml",ios_base::app);
			fout << wr;
			fout.close();
		}
		else
		{
			test.close();
			vector<PPGroup> grps;
			YAML::Node groups = YAML::LoadFile("plugins/PurePerms/groups.yml");
			for (const auto& kv : groups)
			{
				grps.push_back(kv.as<PPGroup>());
			}
			groups.reset();
			for (int i = 0; i < grps.size(); ++i)
			{
				if (grps[i].getName() == oldName)
				{
					grps[i].name = _name;
				}
				groups.push_back(grps[i]);
			}
			remove("plugins/PurePerms/groups.yml");
			ofstream fout("plugins/PurePerms/groups.yml");
			fout << groups;
			fout.close();
		}
	}
	else if (provider == "json")
	{
		//TODO: Алексе здесь уже твой код будет
	}
}

string PPGroup::getAlias() { return alias; }

void PPGroup::setAlias(string _alias)
{
	if (alias == "")
	{
		alias = _alias;
		return;
	}
	string oldAlias = alias;
	alias = _alias;
	YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
	if (config["provider"].as<string>() == "yaml")
	{
		ifstream test("plugins/PurePerms/groups.yml");
		if (!test.is_open())
		{
			test.close();
			YAML::Node wr;
			wr.push_back(*this);
			ofstream fout("plugins/PurePerms/groups.yml");
			fout << wr;
			fout.close();
		}
		else
		{
			test.close();
			vector<PPGroup> grps;
			YAML::Node groups = YAML::LoadFile("plugins/PurePerms/groups.yml");
			for (const auto& kv : groups)
			{
				grps.push_back(kv.as<PPGroup>());
			}
			groups.reset();
			for (int i = 0; i < grps.size(); ++i)
			{
				if (grps[i].alias == oldAlias)
				{
					grps[i].alias = _alias;
				}
				groups.push_back(grps[i]);
			}
			remove("plugins/PurePerms/groups.yml");
			ofstream fout("plugins/PurePerms/groups.yml");
			fout << groups;
			fout.close();
		}
	}
	else if (config["provider"].as<string>() == "json")
	{
		//TODO: Алексе здесь уже твой код будет
	}
}

int PPGroup::IsDefault() { return isDefault; }

void PPGroup::setDefault(int b)
{
	if (isDefault == -1)
	{
		isDefault = b;
		return;
	}
	isDefault = b;
}

void PPGroup::addGroup(string group)
{
	inheritance.push_back(group);
	YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
	if (config["provider"].as<string>() == "yaml")
	{
		ifstream test("plugins/PurePerms/groups.yml");
		if (!test.is_open())
		{
			test.close();
			YAML::Node wr;
			wr.push_back(*this);
			ofstream fout("plugins/PurePerms/groups.yml");
			fout << wr;
			fout.close();
		}
		else
		{
			vector<PPGroup> grps;
			YAML::Node groups = YAML::LoadFile("plugins/PurePerms/groups.yml");
			for (const auto& kv : groups)
			{
				grps.push_back(kv.as<PPGroup>());
			}
			for (int i = 0; i < grps.size(); ++i)
			{
				if (grps[i].name == name)
				{
					grps[i].inheritance.push_back(group);
					groups.reset();
					for (auto g : grps)
						groups.push_back(g);
					remove("plugins/PurePerms/groups.yml");
					ofstream fout("plugins/PurePerms/groups.yml");
					fout << groups;
					fout.close();
					break;
				}
			}
		}
	}
	else if (config["provider"].as<string>() == "json")
	{
		//TODO: Алексе здесь уже твой код будет
	}
}

void PPGroup::removeGroup(string name)
{
	vector<string> result;
	for (auto gr : inheritance)
	{
		if (gr == name)
		{
			continue;
		}
		result.push_back(gr);
	}
	inheritance = result;
	YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
	if (config["provider"].as<string>() == "yaml")
	{
		ifstream test("plugins/PurePerms/groups.yml");
		if (!test.is_open())
		{
			test.close();
			YAML::Node wr;
			wr.push_back(*this);
			ofstream fout("plugins/PurePerms/groups.yml");
			fout << wr;
			fout.close();
		}
		else
		{
			YAML::Node groups = YAML::LoadFile("plugins/PurePerms/groups.yml");
			vector<PPGroup> grps;
			for (const auto& kv : groups)
			{
				grps.push_back(kv.as<PPGroup>());
			}
			for (int i = 0; i < grps.size(); ++i)
			{
				if (grps[i].name == this->name)
				{
					vector<string> res;
					for (auto sd : grps[i].inheritance)
					{
						if (sd == name)
							continue;
						res.push_back(sd);
					}
					grps[i].inheritance = res;
					groups.reset();
					for (auto g : grps)
						groups.push_back(g);
					remove("plugins/PurePerms/groups.yml");
					ofstream fout("plugins/PurePerms/groups.yml");
					fout << groups;
					fout.close();
					break;
				}
			}
		}
	}
	else if (config["provider"].as<string>() == "json")
	{
		//TODO: Алексе здесь уже твой код будет
	}
}

void PPGroup::addPermission(string permission)
{
	permissions.push_back(permission);
	YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
	if (config["provider"].as<string>() == "yaml")
	{
		ifstream test("plugins/PurePerms/groups.yml");
		if (!test.is_open())
		{
			test.close();
			YAML::Node wr;
			wr.push_back(*this);
			ofstream fout("plugins/PurePerms/groups.yml");
			fout << wr;
			fout.close();
		}
		else
		{
			YAML::Node groups = YAML::LoadFile("plugins/PurePerms/groups.yml");
			vector<PPGroup> grps;
			for (const auto& kv : groups)
			{
				grps.push_back(kv.as<PPGroup>());
			}
			for (int i = 0; i < grps.size(); ++i)
			{
				if (grps[i].name == name)
				{
					grps[i].permissions.push_back(permission);
					groups.reset();
					for (auto g : grps)
						groups.push_back(g);
					remove("plugins/PurePerms/groups.yml");
					ofstream fout("plugins/PurePerms/groups.yml");
					fout << groups;
					fout.close();
					break;
				}
			}
		}
	}
	else if (config["provider"].as<string>() == "json")
	{
		//TODO: Алексе здесь уже твой код будет
	}
}

void PPGroup::removePermission(string permission)
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
	YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
	if (config["provider"].as<string>() == "yaml")
	{
		ifstream test("plugins/PurePerms/groups.yml");
		if (!test.is_open())
		{
			test.close();
			YAML::Node wr;
			wr.push_back(*this);
			ofstream fout("plugins/PurePerms/groups.yml");
			fout << wr;
			fout.close();
		}
		else
		{
			YAML::Node groups = YAML::LoadFile("plugins/PurePerms/groups.yml");
			vector<PPGroup> grps;
			for (const auto& kv : groups)
			{
				grps.push_back(kv.as<PPGroup>());
			}
			groups.reset();
			for (int i = 0; i < grps.size(); ++i)
			{
				if (grps[i].name == name)
				{
					grps[i].permissions = result;
					remove("plugins/PurePerms/groups.yml");
					ofstream fout("plugins/PurePerms/groups.yml");
					fout << groups;
					fout.close();
					break;
				}
			}
		}
	}
	else if (config["provider"].as<string>() == "json")
	{
		//TODO: Алексе здесь уже твой код будет
	}
}

vector<string> PPGroup::getInheritances() { return inheritance; }

vector<string> PPGroup::getPermissions()
{
	return permissions;
}

vector<string> PPGroup::getAllPermissions() 
{ 
	vector<string> perms = permissions;
	if (inheritance.size() > 0)
	{
		for (auto inh : inheritance)
		{
			auto in = getGroup(inh);
			auto parentPermissions = in.getPermissions();
			if (parentPermissions.size() == 0)
				continue;
			perms.insert(perms.end(), parentPermissions.begin(), parentPermissions.end());
		}
	}
	return perms;
}


YAML::Node YAML::convert<PPGroup>::encode(const PPGroup& rhs) 
{
	Node node;
	node[((PPGroup)rhs).getName()] = YAML::Node(YAML::NodeType::Map);
	node[((PPGroup)rhs).getName()]["alias"] = ((PPGroup)rhs).getAlias();
	node[((PPGroup)rhs).getName()]["isDefault"] = ((PPGroup)rhs).IsDefault();
	if (((PPGroup)rhs).getInheritances().size() == 0)
		node[((PPGroup)rhs).getName()]["inheritance"] = vector<string>();
	else
	{
		for (auto inh : ((PPGroup)rhs).getInheritances())
		{
			node[((PPGroup)rhs).getName()]["inheritance"].push_back(inh);
		}
	}
	if (((PPGroup)rhs).getPermissions().size() == 0)
		node[((PPGroup)rhs).getName()]["permissions"] = vector<string>();
	else
	{
		for (auto inh : ((PPGroup)rhs).getPermissions())
		{
			node[((PPGroup)rhs).getName()]["permissions"].push_back(inh);
		}
	}
	return node;
}

 bool YAML::convert<PPGroup>::decode(const Node& node, PPGroup& rhs)
 {
	 string name;
	 for (const auto& kv : node)
	 {
		 if (kv.first.as<std::string>() == "")
		 {
			 continue;
		 }
		 name = kv.first.as<std::string>();
		 break;
	 }
	rhs.name = name;
	rhs.alias = node[name]["alias"].as<string>();
	rhs.isDefault = node[name]["isDefault"].as<int>();
	for (auto v : node[name]["inheritance"].as<vector<string>>())
		rhs.inheritance.push_back(v);
	for (auto v : node[name]["permissions"].as<vector<string>>())
		rhs.permissions.push_back((v));
	return true;
}

 PPGroup getGroup(string name)
 {
	 PPGroup pp;
	 YAML::Node config = YAML::LoadFile("plugins/PurePerms/config.yml");
	 if (config["provider"].as<string>() == "yaml")
	 {
		 ifstream test("plugins/PurePerms/groups.yml");
		 if (!test.is_open())
		 {
			 for (auto g : groups)
			 {
				 if (g->getName() == name)
				 {
					 pp = *g;
					 return pp;
				 }
			 }
		 }
		 else
		 {
			 YAML::Node groups = YAML::LoadFile("plugins/PurePerms/groups.yml");
			 for (auto gr : groups)
			 {
				 auto gro = gr.as<PPGroup>();
				 if (gro.getName() == name)
				 {
					 return gro;
				 }
			 }
		 }
		 return pp;
	 }
	 else if (config["provider"].as<string>() == "json")
	 {
		 //TODO: Алексе здесь уже твой код будет
	 }
 }