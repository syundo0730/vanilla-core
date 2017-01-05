#ifndef ROSBRIDGE_API_H
#define ROSBRIDGE_API_H

#include "json.hpp"
#include "WebsocketApi.h"

#include <memory>

class ROSBridgeApi
{
public:
	static std::unique_ptr<ROSBridgeApi> instantiate(WebsocketApi &websocketApi);
	using json = nlohmann::json;
	virtual void write(const json &j) = 0;
};

#endif // ROSBRIDGE_API_H