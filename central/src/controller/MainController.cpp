#include "MainController.h"
#include "dimensional_types.h"
#include "JointRepository.h"
#include "WalkController.h"
#include "MotionController.h"
#include "MotionSensor.h"
#include "CommandBus.h"
#include "CommandParser.h"
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

  public:
	MainControllerImpl(
		JointRepository &joint_repository,
		WalkController &walk_controller,
		MotionController &motionController,
		MotionSensor &motion_sensor,
		CommandBus &command_bus,
		CommandParser &commandParser)
		: joint_repository(joint_repository),
		  walk_controller(walk_controller),
		  motionController(motionController),
		  motion_sensor(motion_sensor),
		  command_bus(command_bus),
		  commandParser(commandParser)
	{
	}
	void update() override
	{
		walk_controller.update();
		motionController.update();
		joint_repository.applyTargetAngle();

		// // get motion state
		// auto q = motion_sensor.getQuaternion();
		// auto pos = Vector3(1, 0, 0);
		// auto newPos = q * pos;
		// // std::cout << newPos[0] << std::endl;
		// // auto ea = q.toRotationMatrix().eulerAngles(0, 1, 2);
		// std::cout << newPos(0) << "\t";		 //roll
		// std::cout << newPos(1) << "\t";		 //pitch
		// std::cout << newPos(2) << std::endl; //yaw
	}
	void route(const uint8_t *data, std::size_t length) override
	{
		Command cmd;
		try {
			cmd = commandParser.parse(data, length);
		} catch (const std::runtime_error& e) {
			std::cout << e.what() << std::endl;
			return;
		}
		command_bus.publish(cmd);
	}
};

std::unique_ptr<MainController> MainController::instantiate(
	JointRepository &jointRepository,
	WalkController &walkController,
	MotionController &motionController,
	MotionSensor &motionSensor,
	CommandBus &commandBus,
	CommandParser &commandParser)
{
	return std::make_unique<MainControllerImpl>(
		jointRepository,
		walkController,
		motionController,
		motionSensor,
		commandBus,
		commandParser);
}