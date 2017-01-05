#ifndef TICKER_H_
#define TICKER_H_

#include <boost/asio.hpp>
#include <functional>

class Ticker {
private:
	boost::asio::io_service io_service;
	boost::posix_time::milliseconds interval;
	boost::asio::deadline_timer timer;
	std::function<void()> on_tick;

public:
	Ticker(unsigned int interval_milli_sec, std::function<void()> listener);

public:
	void start();
	void stop();

private:
	void tick(const boost::system::error_code& err);
};

#endif
