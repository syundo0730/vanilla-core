#ifndef CONV_LEG_KINEMATICS_CONF_H
#define CONV_LEG_KINEMATICS_CONF_H

#include "yaml-cpp/yaml.h"
#include "conf.h"
#include "conv_vector3d.h"

namespace YAML
{
template <>
struct convert<LegKinematicsConf>
{
	static Node encode(const LegKinematicsConf &rhs)
	{
		Node node;
		node["l1"] = rhs.L1;
		node["l2"] = rhs.L2;
		node["leg_root_left"] = rhs.LegRootLeft;
		node["leg_root_right"] = rhs.LegRootRight;
		return node;
	}

	static bool decode(const Node &node, LegKinematicsConf &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.L1 = node["l1"].as<double>();
		rhs.L2 = node["l2"].as<double>();
		rhs.LegRootLeft = node["leg_root_left"].as<Vector3>();
		rhs.LegRootRight = node["leg_root_right"].as<Vector3>();
		return true;
	}
};
}

#endif
