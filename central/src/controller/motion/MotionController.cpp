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
		} else if (command.commandType == CommandType::SetJointAngle) {
			int id = command.payload[0];
			int16_t angle = command.payload[1];
			motionService.setTargetJointAngle(id, angle);
		} else if (command.commandType == CommandType::ShowJointAngles) {
			auto angles = motionService.getCurrentJointAngles();
			std::vector<int16_t> anglesVec(angles.size());
			for (const auto &kv : angles)
			{
				auto id = kv.first;
				auto value = kv.second;
				anglesVec[id] = value;
			}
			int i = 0;
			for (const auto v : anglesVec)
			{
				if (i == 0) {
					std::cout << v;
				} else {
					std::cout << "," << v;
				}
				++i;
			}
			std::cout << std::endl;
		} else if (command.commandType == CommandType::LoadConfig) {
			std::cout << "motion reloading ..." << std::endl;
			motionService.reloadMotion();
			std::cout << "motion reloaded" << std::endl;
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