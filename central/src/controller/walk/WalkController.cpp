#include "WalkController.h"
#include "CommandBus.h"
#include "WalkGenerator.h"
#include "LegControlService.h"
#include <functional>
#include <iostream>

class WalkControllerImpl : public WalkController
{
  private:
	CommandBus &commandBus;
	WalkGenerator &walkGenerator;
	LegControlService &legControlService;

  public:
	WalkControllerImpl(
		CommandBus &commandBus,
		WalkGenerator &walkGenerator,
		LegControlService &legControlService)
		: commandBus(commandBus),
		  walkGenerator(walkGenerator),
		  legControlService(legControlService)
	{
		commandBus.subscribe(
			std::bind(&WalkControllerImpl::onCommand, this, std::placeholders::_1));
	}
	void update() override
	{
		if (!walkGenerator.needsUpdate()) {
			return;
		}
		walkGenerator.update();
		auto state = walkGenerator.getState();
		legControlService.setEndEffectorPosition(
			state.left_leg_position - state.body_position,
			state.right_leg_position - state.body_position);
	}

  private:
	void onCommand(Command command)
	{
		switch (command.commandType)
		{
		case CommandType::WalkStart:
			walkGenerator.start();
			break;
		case CommandType::WalkStop:
			walkGenerator.stop();
			break;
		}
	}
};

std::unique_ptr<WalkController> WalkController::instantiate(
	CommandBus &commandBus,
	WalkGenerator &walkGenerator,
	LegControlService &legControlService)
{
	return std::make_unique<WalkControllerImpl>(
		commandBus,
		walkGenerator,
		legControlService);
}