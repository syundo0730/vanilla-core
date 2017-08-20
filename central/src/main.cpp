#include "Ticker.h"
#include "Dependency.h"
#include "MainController.h"
#include "SerialApi.h"
#include <iostream>
#include <functional>
#include <memory>
#include <thread>
#include <exception>

int main()
{
	auto dependency = Dependency::instantiate();

	// command watch thread
	auto stdIORouter = dependency->getStdIORouter();
	try {
		std::thread th([&]{
			std::cout << "waiting command" << std::endl;
			while(true) {
				std::cout << "Enter command:" << std::endl;
				stdIORouter->route();
			}
		});
		th.join();
	} catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}

	auto controller = dependency->getMainController();

	// bluetooth command watch thread
	auto bluetooth = std::make_unique<SerialApi>("hoge", 115200);
	// SerialApi::Listener listener = [](const uint8_t* data, std::size_t size) {
	// 	controller->route(data, size);
	// };
	// bluetooth->setListener(listener);
	bluetooth->setListener(std::bind(
		&MainController::route,
		controller,
		std::placeholders::_1,
		std::placeholders::_2));

	// control loop
	Ticker ticker(10, std::bind(&MainController::update, controller));
	ticker.start();
}