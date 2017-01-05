#ifndef WEBSOCKET_API_H
#define WEBSOCKET_API_H

#include <string>
#include <functional>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
 
class WebsocketApi
{
  public:
	using Listener = std::function<void(char *)>;
	using Client = websocketpp::client<websocketpp::config::asio_client>;
	using message_ptr = websocketpp::config::asio_client::message_type::ptr;
  private:
  	Client client;
  public:
	WebsocketApi(std::string uri = "");
	void onMessage(websocketpp::connection_hdl hdl, message_ptr msg);
	void write();
};
 
#endif // WEBSOCKET_API_H