#include "Ticker.h"
#include "Dependency.h"
#include "MainController.h"
#include <iostream>
#include <thread>
#include <exception>

int main()
{
	auto dependency = Dependency::instantiate();

	// command watch loop
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

	// control loop
	auto controller = dependency->getMainController();
	Ticker ticker(10, std::bind(&MainController::update, controller));
	ticker.start();
}