#ifndef MOTION_H
#define MOTION_H

#include "yaml-cpp/yaml.h"
#include <cstdint>
#include <string>
#include <sstream>

struct MotionPose {
    int Interval;
    std::vector<int16_t> JointAnglesDeciDegree;
};

struct Motion {
    int ID;
    std::vector<MotionPose> Poses;
};

namespace YAML
{
template <>
struct convert<MotionPose>
{
    static std::vector<int16_t> strToVector(std::string src)
    {
        std::vector<int16_t> dist;
        std::stringstream ss(src);
        int16_t i;
        while (ss >> i)
        {
            dist.push_back(i);
            if (ss.peek() == ',')
                ss.ignore();
        }
        return dist;
    }

    static std::string vectorToStr(std::vector<int16_t> &src) {
        std::string str = "";
        for (auto & v: src) {
            str += std::to_string(v);
        }
        return str;
    }

    static Node encode(const MotionPose &rhs)
	{
		Node node;
		node["interval"] = rhs.Interval;
        auto vec = rhs.JointAnglesDeciDegree;
		node["joint_angles_deci_degree"] = vectorToStr(vec);
		return node;
	}

	static bool decode(const Node &node, MotionPose &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.Interval = node["interval"].as<int16_t>();
        auto str = node["joint_angles_deci_degree"].as<std::string>();
		rhs.JointAnglesDeciDegree = strToVector(str);
		return true;
	}
};
template <>
struct convert<Motion>
{
    static Node encode(const Motion &rhs)
	{
		Node node;
		node["id"] = rhs.ID;
		node["poses"] = rhs.Poses;
		return node;
	}

	static bool decode(const Node &node, Motion &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.ID = node["id"].as<int>();
		rhs.Poses = node["poses"].as<std::vector<MotionPose>>();
		return true;
	}
};
}

#endif
