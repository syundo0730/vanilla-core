#ifndef CONV_JOINT_CONF_H
#define CONV_JOINT_CONF_H

#include "yaml-cpp/yaml.h"
#include <string>
#include <map>
#include "conf.h"
#include "conv_joint_setting.h"

namespace YAML
{
template <>
struct convert<JointConf>
{
	static Node encode(const JointConf &rhs)
	{
		Node node;
		node["id_map"] = rhs.IDMap;
		node["setting_map"] = rhs.SettingMap;
		return node;
	}

	static bool decode(const Node &node, JointConf &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.IDMap = node["id_map"].as<std::map<std::string, int>>();
		rhs.SettingMap = node["setting_map"].as<std::map<int, JointSetting>>();
		return true;
	}
};
}

#endif
