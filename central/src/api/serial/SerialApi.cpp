#include "SerialApi.h"
#include <iostream>
#include <functional>
#include <thread>
#include <boost/bind.hpp>
#include <boost/array.hpp>

SerialApi::SerialApi(std::string port_path, unsigned int baud_rate)
{
    io = std::unique_ptr<boost::asio::io_service>(new boost::asio::io_service());
    port = std::unique_ptr<boost::asio::serial_port>(new boost::asio::serial_port(*io));

    boost::system::error_code err;
    port->open(port_path.c_str(), err);

    if (err)
    {
        std::cerr << "Could not found any serial port!" << std::endl;
        exit(1);
    }
    else
    {
        port->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        port->set_option(boost::asio::serial_port_base::character_size(8));
        port->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
        port->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        port->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

        std::cout << "port: " << port_path << " Opend!" << std::endl;
    }
}

SerialApi::~SerialApi()
{
    port->close();
}

void SerialApi::write(std::vector<unsigned char> arg)
{
    std::string str(arg.begin(), arg.end());
    port->write_some(boost::asio::buffer(str));
}

void SerialApi::setListener(Listener listener)
{
    // thread for reading
    std::thread thr_io(boost::bind(&boost::asio::io_service::run, io.get()));
    thr_io.join();

    boost::array<uint8_t, 128> rbuf;
    std::function<void(const boost::system::error_code, std::size_t)> onRead = [&](
        const boost::system::error_code &e,
        std::size_t size) {
        listener(rbuf.data(), size);
        port->async_read_some(boost::asio::buffer(rbuf), onRead);
    };
    port->async_read_some(boost::asio::buffer(rbuf), onRead);
}