#ifndef CONV_CP_STABILIZER_CONF_H
#define CONV_CP_STABILIZER_CONF_H

#include "yaml-cpp/yaml.h"
#include <string>
#include "conf.h"

namespace YAML
{
template <>
struct convert<CPStabilizerConf>
{
	static Node encode(const CPStabilizerConf &rhs)
	{
		Node node;
		node["omega"] = rhs.Omega;
		node["time_shift"] = rhs.TimeShift;
		return node;
	}

	static bool decode(const Node &node, CPStabilizerConf &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.Omega = node["omega"].as<double>();
		rhs.TimeShift = node["time_shift"].as<double>();
		return true;
	}
};
}

#endif
