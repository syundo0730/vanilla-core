#ifndef CONV_SERIAL_PORT_CONF_H
#define CONV_SERIAL_PORT_CONF_H

#include "yaml-cpp/yaml.h"
#include <string>
#include "conf.h"

namespace YAML
{
template <>
struct convert<SerialPortConf>
{
	static Node encode(const SerialPortConf &rhs)
	{
		Node node;
		node["port_path"] = rhs.PortPath;
		node["baud_rate"] = rhs.BaudRate;
		return node;
	}

	static bool decode(const Node &node, SerialPortConf &rhs)
	{
		if (!node.IsMap())
		{
			return false;
		}

		rhs.PortPath = node["port_path"].as<std::string>();
		rhs.BaudRate = node["baud_rate"].as<unsigned int>();
		return true;
	}
};
}

#endif
