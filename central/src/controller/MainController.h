#ifndef MAIN_CONTROLLER_H_
#define MAIN_CONTROLLER_H_

#include <memory>
#include <cstdint>
class JointRepository;
class WalkController;
class MotionController;
class MotionSensor;
class CommandBus;
class CommandParser;

class MainController
{
  public:
	static std::unique_ptr<MainController> instantiate(
		JointRepository &,
		WalkController &,
		MotionController &,
		MotionSensor &,
		CommandBus &,
		CommandParser &);
	virtual void update() = 0;
	virtual void route(const uint8_t *data, std::size_t length) = 0;
};

#endif
