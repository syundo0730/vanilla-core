#ifndef CONV_CONF_H
#define CONV_CONF_H

#include "yaml-cpp/yaml.h"
#include "conf.h"
#include "conv_cp_stabilizer_conf.h"
#include "conv_leg_kinematics_conf.h"
#include "conv_serial_port_conf.h"
#include "conv_system_conf.h"
#include "conv_walk_conf.h"
#include "conv_joint_conf.h"

namespace YAML
{
template <>
struct convert<Conf>
{
	static Node encode(const Conf &rhs)
	{
		Node node;
		node["cp_stabilizer"] = rhs.CPStabilizer;
		node["leg_kinematics"] = rhs.LegKinematics;
		node["serial_port"] = rhs.SerialPort;
		node["system"] = rhs.System;
		node["walk"] = rhs.Walk;
		node["joint"] = rhs.Joint;
		return node;
	}

	static bool decode(const Node &node, Conf &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.CPStabilizer = node["cp_stabilizer"].as<CPStabilizerConf>();
		rhs.LegKinematics = node["leg_kinematics"].as<LegKinematicsConf>();
		rhs.SerialPort = node["serial_port"].as<SerialPortConf>();
		rhs.System = node["system"].as<SystemConf>();
		rhs.Walk = node["walk"].as<WalkConf>();
		rhs.Joint = node["joint"].as<JointConf>();
		return true;
	}
};
}

#endif
