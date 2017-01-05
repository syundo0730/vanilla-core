#ifndef SERIAL_API_H
#define SERIAL_API_H

#include <boost/asio.hpp>
#include <vector>
#include <string>

class SerialApi
{
public:
	using Listener = std::function<void(char *)>;

private:
	std::unique_ptr<boost::asio::io_service> io;
	std::unique_ptr<boost::asio::serial_port> port;

public:
	SerialApi(std::string port_path, unsigned int baud_rate);
	virtual ~SerialApi();
	void write(std::vector<unsigned char> arg);
	void setListener(Listener listener);
};

#endif // SERIAL_API_H