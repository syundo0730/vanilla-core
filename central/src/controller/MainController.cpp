#include "MainController.h"
#include "dimensional_types.h"
#include "JointRepository.h"
#include "WalkController.h"
#include "MotionController.h"
#include "MotionSensor.h"
#include "CommandBus.h"
#include "CommandParser.h"
#include "Conf.h"
#include <iostream>
#include <cstdint>

class MainControllerImpl : public MainController
{
  private:
	JointRepository &joint_repository;
	WalkController &walk_controller;
	MotionController &motionController;
	MotionSensor &motion_sensor;
	CommandBus &command_bus;
	CommandParser &commandParser;
	Conf &conf;

  public:
	MainControllerImpl(
		JointRepository &joint_repository,
		WalkController &walk_controller,
		MotionController &motionController,
		MotionSensor &motion_sensor,
		CommandBus &command_bus,
		CommandParser &commandParser,
		Conf &conf)
		: joint_repository(joint_repository),
		  walk_controller(walk_controller),
		  motionController(motionController),
		  motion_sensor(motion_sensor),
		  command_bus(command_bus),
		  commandParser(commandParser),
		  conf(conf)
	{
		command_bus.subscribe(
			std::bind(&MainControllerImpl::onCommand, this, std::placeholders::_1));
	}
	void update() override
	{
		walk_controller.update();
		motionController.update();
		joint_repository.applyTargetAngle();

		// get motion state
		auto q = motion_sensor.getQuaternion();
		auto pos = Vector3(1, 0, 0);
		auto newPos = q * pos;
		// std::cout << newPos[0] << std::endl;
		// auto ea = q.toRotationMatrix().eulerAngles(0, 1, 2);
		// std::cout << newPos(0) << "\t";		 //roll
		// std::cout << newPos(1) << "\t";		 //pitch
		// std::cout << newPos(2) << std::endl; //yaw
	}
	void route(const uint8_t *data, std::size_t length) override
	{
		std::cout << data << std::endl;
		Command cmd;
		try
		{
			cmd = commandParser.parse(data, length);
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			return;
		}
		command_bus.publish(cmd);
	}

  private:
	void onCommand(Command command)
	{
		if (command.commandType == CommandType::LoadConfig)
		{
			std::cout << "config file reloaded" << std::endl;
			conf.reload();
			std::cout << "move to initial pose" << std::endl;
			joint_repository.moveToInitialPose();
		}
	}
};

std::unique_ptr<MainController> MainController::instantiate(
	JointRepository &jointRepository,
	WalkController &walkController,
	MotionController &motionController,
	MotionSensor &motionSensor,
	CommandBus &commandBus,
	CommandParser &commandParser,
	Conf &conf)
{
	return std::make_unique<MainControllerImpl>(
		jointRepository,
		walkController,
		motionController,
		motionSensor,
		commandBus,
		commandParser,
		conf);
}