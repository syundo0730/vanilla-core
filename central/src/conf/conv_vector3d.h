#ifndef CONV_VECTOR3D_H
#define CONV_VECTOR3D_H

#include "yaml-cpp/yaml.h"
#include "dimensional_types.h"
#include <string>
#include "conf.h"
#include "conv_vector3d.h"

namespace YAML
{
template <>
struct convert<Vector3>
{
	static Node encode(const Vector3 &rhs)
	{
		Node node;
		node.push_back(rhs[0]);
		node.push_back(rhs[1]);
		node.push_back(rhs[2]);
		return node;
	}

	static bool decode(const Node &node, Vector3 &rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
		{
			return false;
		}

		rhs[0] = node[0].as<double>();
		rhs[1] = node[1].as<double>();
		rhs[2] = node[2].as<double>();
		return true;
	}
};
}

#endif
