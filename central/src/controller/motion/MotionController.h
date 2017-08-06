#ifndef MOTION_CONTROLLER_H_
#define MOTION_CONTROLLER_H_

#include <memory>

class CommandBus;
class MotionService;

class MotionController {
  public:
	static std::unique_ptr<MotionController> instantiate(
        CommandBus &commandBus,
        MotionService &motionService);
	virtual void update() = 0;
};

#endif
