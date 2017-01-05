#ifndef CONV_SYSTEM_CONF_H
#define CONV_SYSTEM_CONF_H

#include "yaml-cpp/yaml.h"
#include <string>
#include "conf.h"

namespace YAML
{
template <>
struct convert<SystemConf>
{
	static Node encode(const SystemConf &rhs)
	{
		Node node;
		node["interval_sec"] = rhs.IntervalSec;
		node["g_acceleration"] = rhs.GAcceleration;
		return node;
	}

	static bool decode(const Node &node, SystemConf &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.IntervalSec = node["interval_sec"].as<double>();
		rhs.GAcceleration = node["g_acceleration"].as<double>();
		return true;
	}
};
}

#endif
