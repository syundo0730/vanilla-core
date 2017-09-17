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
	std::thread commandThread([&] {
		std::cout << "waiting command" << std::endl;
		while (true)
		{
			std::cout << "Enter command:" << std::endl;
			stdIORouter->route();
		}
	});

	auto controller = dependency->getMainController();
	// main control thread
	std::thread mainControlThread([&] {
		std::cout << "main control thread started" << std::endl;
		// control loop
		Ticker ticker(10, std::bind(&MainController::update, controller));
		ticker.start();
	});

	auto autoController = dependency->getAutoController();
	// auto control thread
	std::thread autoControlThread([&] {
		std::cout << "auto control thread started" << std::endl;
		// control loop
		Ticker ticker(20, std::bind(&AutoController::update, autoController));
		ticker.start();
	});

	// // bluetooth command watch thread
	// auto bluetooth = std::make_unique<SerialApi>("hoge", 115200);
	// bluetooth->setListener(std::bind(
	// 	&MainController::route,
	// 	controller,
	// 	std::placeholders::_1,
	// 	std::placeholders::_2));

	commandThread.join();
	mainControlThread.join();
	autoControlThread.join();
}