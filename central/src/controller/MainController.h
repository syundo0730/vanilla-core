#ifndef MAIN_CONTROLLER_H_
#define MAIN_CONTROLLER_H_

#include <memory>
class JointRepository;
class WalkController;
class MotionSensor;
class CommandBus;
class CommandParser;

class MainController
{
  public:
	static std::unique_ptr<MainController> instantiate(
		JointRepository &,
		WalkController &,
		MotionSensor &,
		CommandBus &,
		CommandParser &);
	virtual void update() = 0;
	virtual void route(const char *data, int length) = 0;
};

#endif
