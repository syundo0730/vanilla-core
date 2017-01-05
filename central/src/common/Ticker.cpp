#include "Ticker.h"
#include <iostream>
#include <boost/bind.hpp>

Ticker::Ticker(unsigned int interval_milli_sec, std::function<void()> listener):
	interval(interval_milli_sec), timer(io_service, interval), on_tick(listener)  {
}

void Ticker::start() {
    // Schedule the timer for the first time:
	timer.async_wait(boost::bind(&Ticker::tick, this, boost::asio::placeholders::error));
    // Enter IO loop. The timer will fire for the first time 1 second from now:
    io_service.run();
}

void Ticker::stop() {}

void Ticker::tick(const boost::system::error_code& err) {
	// call listener
	on_tick();
	// Reschedule the timer for interval in the future:
	timer.expires_at(timer.expires_at() + interval);
	// Posts the timer event
	timer.async_wait(boost::bind(&Ticker::tick, this, boost::asio::placeholders::error));
}
