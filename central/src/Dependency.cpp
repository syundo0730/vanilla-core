#include "Dependency.h"

#include "CommandBus.h"
#include "CommandParser.h"
#include "mraa.hpp"
#include "I2Cdev.h"
#include "SerialApi.h"
#include "RSApi.h"
#include "MPU6050.h"
#include "Adafruit_PWMServoDriver.h"
#include "MadgwickQuaternionFilter.h"
#include "MotionSensor.h"
#include "JointRepository.h"
#include "LegKinematics.h"
#include "LegControlService.h"
#include "LinearInvertedPendulum.h"
#include "SwingLegTrajectory.h"
#include "CPStabilizer.h"
#include "WalkGenerator.h"
#include "WalkController.h"
#include "MotionLoader.h"
#include "MotionService.h"
#include "MotionController.h"
#include "StdIOCommandParser.h"
#include "StdIORouter.h"
#include "conf.h"
#include "RangeSensor.h"
#include "RangeSensorArray.h"
#include "ActiveRangeSensorArray.h"

class DependencyImpl : public Dependency
{
  private:
	std::unique_ptr<Conf> conf;
	std::unique_ptr<CommandBus> commandBus;
	std::unique_ptr<CommandParser> commandParser;
	std::unique_ptr<mraa::I2c> i2c;
	std::unique_ptr<I2Cdev> i2cdev;
	std::unique_ptr<SerialApi> serialApi;
	std::unique_ptr<RSApi> rsApi;
	std::unique_ptr<MPU6050> mpu;
	std::unique_ptr<Adafruit_PWMServoDriver> adafruit_PWMServoDriver;
	std::unique_ptr<MadgwickQuaternionFilter> madgwickQuaternionFilter;
	std::unique_ptr<MotionSensor> motionSensor;
	std::unique_ptr<JointRepository> jointRepository;
	std::unique_ptr<LegKinematics>
		leftLegKinematics,
		rightLegKinematics;
	std::unique_ptr<LegControlService> legControlService;
	std::unique_ptr<LinearInvertedPendulum> linearInvertedPendulum;
	std::unique_ptr<SwingLegTrajectory> swingLegTrajectory;
	std::unique_ptr<CPStabilizer> cpStabilizer;
	std::unique_ptr<WalkGenerator> walkGenerator;
	std::unique_ptr<WalkController> walkController;
	std::unique_ptr<MotionLoader> motionLoader;
	std::unique_ptr<MotionService> motionService;
	std::unique_ptr<MotionController> motionController;
	std::unique_ptr<MainController> mainController;

	std::unique_ptr<StdIOCommandParser> stdIOCommandParser;
	std::unique_ptr<StdIORouter> stdIORouter;

	std::unique_ptr<RangeSensorArray> rangeSensorArray;
	std::unique_ptr<ActiveRangeSensorArray> activeRangeSensorArray;
	std::unique_ptr<AutoController> autoController;

  public:
	DependencyImpl()
		: conf(std::make_unique<Conf>("config.yaml")),
		  commandBus(CommandBus::instantiate()),
		  commandParser(CommandParser::instantiate()),
		  i2c(std::make_unique<mraa::I2c>(1, true)),
		  i2cdev(std::make_unique<I2Cdev>(*i2c)),
		  serialApi(std::make_unique<SerialApi>(
			  conf->SerialPort.PortPath,
			  conf->SerialPort.BaudRate)),
		  rsApi(RSApi::instantiate(*serialApi)),
		  mpu(std::make_unique<MPU6050>(*i2cdev)),
		  adafruit_PWMServoDriver(Adafruit_PWMServoDriver::instantiate(*i2cdev)),
		  madgwickQuaternionFilter(std::make_unique<MadgwickQuaternionFilter>()),
		  motionSensor(MotionSensor::instantiate(*mpu, *madgwickQuaternionFilter)),
		  jointRepository(JointRepository::instantiate(*rsApi, *adafruit_PWMServoDriver, *conf)),
		  leftLegKinematics(LegKinematics::instantiate(conf->LegKinematics.LegRootLeft, *conf)),
		  rightLegKinematics(LegKinematics::instantiate(conf->LegKinematics.LegRootRight, *conf)),
		  legControlService(LegControlService::instantiate(
			  *jointRepository, *leftLegKinematics, *rightLegKinematics, *conf)),
		  linearInvertedPendulum(LinearInvertedPendulum::instantiate(*conf)),
		  swingLegTrajectory(SwingLegTrajectory::instantiate(*conf)),
		  cpStabilizer(CPStabilizer::instantiate(*conf)),
		  walkGenerator(WalkGenerator::instantiate(
			  *linearInvertedPendulum, *swingLegTrajectory, *cpStabilizer, *conf)),
		  walkController(WalkController::instantiate(
			  *commandBus, *walkGenerator, *legControlService)),
		  motionLoader(MotionLoader::instantiate()),
		  motionService(MotionService::instantiate(
			  *motionLoader, *jointRepository, *conf)),
		  motionController(MotionController::instantiate(
			  *commandBus, *motionService)),
		  mainController(MainController::instantiate(
			  *jointRepository, *walkController, *motionController, *motionSensor, *commandBus, *commandParser)),
		  stdIOCommandParser(StdIOCommandParser::instantiate()),
		  stdIORouter(StdIORouter::instantiate(*commandBus, *stdIOCommandParser)),
		  rangeSensorArray(RangeSensorArray::instantiate(*i2cdev)),
		  activeRangeSensorArray(ActiveRangeSensorArray::instantiate(*rangeSensorArray, *jointRepository, 21)),
		  autoController(AutoController::instantiate(*activeRangeSensorArray, *commandBus))
	{
	}

	MainController *getMainController() override
	{
		return mainController.get();
	}

	StdIORouter *getStdIORouter() override
	{
		return stdIORouter.get();
	}

	AutoController *getAutoController() override {
		return autoController.get();
	}
};

std::unique_ptr<Dependency> Dependency::instantiate()
{
	return std::make_unique<DependencyImpl>();
}