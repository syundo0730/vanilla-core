#ifndef CONV_JOINT_SETTING_H
#define CONV_JOINT_SETTING_H

#include "yaml-cpp/yaml.h"
#include "conf.h"

namespace YAML
{
template <>
struct convert<JointSetting>
{
	static Node encode(const JointSetting &rhs)
	{
		Node node;
		node["motor_type"] = static_cast<int>(rhs.MotorType);
		node["real_id"] = rhs.RealID;
		node["offset"] = rhs.Offset;
		return node;
	}

	static bool decode(const Node &node, JointSetting &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.MotorType = static_cast<motor_type>(node["motor_type"].as<int>());
		rhs.RealID = node["real_id"].as<int>();
		rhs.Offset = node["offset"].as<int>();
		return true;
	}
};
}

#endif
