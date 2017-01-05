#ifndef CONV_WALK_CONF_H
#define CONV_WALK_CONF_H

#include "yaml-cpp/yaml.h"
#include <string>
#include "conf.h"

namespace YAML
{
template <>
struct convert<WalkConf>
{
	static Node encode(const WalkConf &rhs)
	{
		Node node;
		node["default_z_c"] = rhs.DefaultZc;
		node["default_t_sup"] = rhs.DefaultTsup;
		node["prediction_step_num"] = rhs.PredictionStepNum;
		node["default_target_horizontal_step_length"] = rhs.DefaultTargetHorizontalStepLength;
		node["default_target_longitudinal_step_length"] = rhs.DefaultTargetLongitudinalStepLength;
		return node;
	}

	static bool decode(const Node &node, WalkConf &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.DefaultZc = node["default_z_c"].as<double>();
		rhs.DefaultTsup = node["default_t_sup"].as<double>();
		rhs.PredictionStepNum = node["prediction_step_num"].as<int>();
		rhs.DefaultTargetHorizontalStepLength = node["default_target_horizontal_step_length"].as<double>();
		rhs.DefaultTargetLongitudinalStepLength = node["default_target_longitudinal_step_length"].as<double>();
		return true;
	}
};
}

#endif
