#include "MotionController.h"
#include "CommandBus.h"
#include "MotionService.h"
#include <functional>
#include <iostream>

class MotionControllerImpl : public MotionController
{
  private:
	CommandBus &commandBus;
	MotionService &motionService;

  public:
	MotionControllerImpl(
		CommandBus &commandBus,
		MotionService &motionService)
		: commandBus(commandBus),
		motionService(motionService)
	{
		commandBus.subscribe(
			std::bind(&MotionControllerImpl::onCommand, this, std::placeholders::_1));
	}
	void update() override
	{
		motionService.update();
	}

  private:
	void onCommand(Command command)
	{
		if (command.commandType == CommandType::MotionStart) {
			int motionID = command.payload.at(0);
			motionService.start(motionID);
		} else if (command.commandType == CommandType::MotionStop) {
			motionService.stop();
		}
	}
};

std::unique_ptr<MotionController> MotionController::instantiate(
	CommandBus &commandBus,
	MotionService &motionService)
{
	return std::make_unique<MotionControllerImpl>(
		commandBus,
		motionService);
}