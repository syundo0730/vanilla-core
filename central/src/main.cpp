#include "Ticker.h"
#include "Dependency.h"

int main()
{
	auto dependency = Dependency::instantiate();
	auto controller = dependency->getMainController();
	Ticker ticker(10, std::bind(&MainController::update, controller));
	ticker.start();
}