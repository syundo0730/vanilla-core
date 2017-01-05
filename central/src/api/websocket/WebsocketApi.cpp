#include "WebsocketApi.h"
#include <iostream>

WebsocketApi::WebsocketApi(std::string _uri)
{
	std::string uri = "ws://localhost:9002";
	if (_uri != "")
	{
		uri = _uri;
	}
	try
	{
		client.set_access_channels(websocketpp::log::alevel::all);
		client.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize ASIO
		client.init_asio();

		// Register our message handler
		client.set_message_handler(
			websocketpp::lib::bind(
				&WebsocketApi::onMessage,
				this,
				websocketpp::lib::placeholders::_1,
				websocketpp::lib::placeholders::_2));

		websocketpp::lib::error_code error_code;
		Client::connection_ptr con = client.get_connection(uri, error_code);
		if (error_code)
		{
			std::cout << "could not create connection because: " << error_code.message() << std::endl;
		}
		client.connect(con);
		client.run();
	}
	catch (websocketpp::exception const &e)
	{
		std::cout << e.what() << std::endl;
	}
}

void WebsocketApi::onMessage(websocketpp::connection_hdl hdl, message_ptr msg)
{
	std::cout << "on_message called with hdl: " << hdl.lock().get()
			  << " and message: " << msg->get_payload()
			  << std::endl;

	websocketpp::lib::error_code ec;

	client.send(hdl, msg->get_payload(), msg->get_opcode(), ec);
	if (ec)
	{
		std::cout << "Echo failed because: " << ec.message() << std::endl;
	}
}

void WebsocketApi::write()
{
}