#include "ROSBridgeApi.h"
#include <iostream>
#include <string>

class ROSBridgeApiImpl : public ROSBridgeApi
{
private:
	WebsocketApi &websocket_api;

public:
	// ROSBridgeApi(std::string _uri)
	ROSBridgeApiImpl(WebsocketApi &websocket_api)
			: websocket_api(websocket_api) {}
	void write(const json &j) override
	{
		auto str = j.dump();
		websocket_api.write();
	}
};

std::unique_ptr<ROSBridgeApi> ROSBridgeApi::instantiate(WebsocketApi &websocketApi)
{
	return std::make_unique<ROSBridgeApiImpl>(websocketApi);
}