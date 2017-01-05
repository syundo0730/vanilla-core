#include "conf.h"
#include "conv_conf.h"
#include "conv_cp_stabilizer_conf.h"
#include "conv_leg_kinematics_conf.h"
#include "conv_serial_port_conf.h"
#include "conv_system_conf.h"
#include "conv_walk_conf.h"
#include <iostream>

Conf::Conf(std::string configFilePath) : Conf(load(configFilePath))
{
}

Conf Conf::load(std::string configFilePath)
{
	return YAML::LoadFile(configFilePath).as<Conf>();
}

Conf Conf::defaultConf()
{
	Conf conf;
	conf.SerialPort = SerialPortConf{
		PortPath : "/dev/ttyS0",
		BaudRate : 115200,
	};
	conf.System = SystemConf{
		IntervalSec : 0.01,
		GAcceleration : 9.81,
	};
	conf.LegKinematics = LegKinematicsConf{
		L1 : 0.045,
		L2 : 0.09,
		LegRootLeft : Vector3(0, 0.03, -0.05),
		LegRootRight : Vector3(0, -0.03, -0.05),
	};
	conf.Walk = WalkConf{
		DefaultZc : 0.8,
		DefaultTsup : 0.8,
		PredictionStepNum : 3,
		DefaultTargetHorizontalStepLength : 0.2,
		DefaultTargetLongitudinalStepLength : 0,
	};
	conf.CPStabilizer = CPStabilizerConf{
		Omega : 0.1,
		TimeShift : 0.1,
	};
	conf.Joint = JointConf{
		IDMap : {
			{"ll0", 0},
			{"ll1", 1},
			{"ll2", 2},
			{"ll3", 3},
			{"ll4", 4},
			{"ll5", 5},
			{"lr0", 6},
			{"lr1", 7},
			{"lr2", 8},
			{"lr3", 9},
			{"lr4", 10},
			{"lr5", 11},
		},
		SettingMap : {
			{0, JointSetting{
				 MotorType : motor_type::RS,
				 RealID : 0,
				 Offset : 0,
			 }},
			{1, JointSetting{
				 MotorType : motor_type::RS,
				 RealID : 1,
				 Offset : 1,
			 }},
			{2, JointSetting{
				 MotorType : motor_type::RS,
				 RealID : 2,
				 Offset : 2,
			 }},
			{3, JointSetting{
				 MotorType : motor_type::RS,
				 RealID : 3,
				 Offset : 3,
			 }},
			{4, JointSetting{
				 MotorType : motor_type::PRS,
				 RealID : 0,
				 Offset : 0,
			 }},
			{5, JointSetting{
				 MotorType : motor_type::PRS,
				 RealID : 1,
				 Offset : 1,
			 }},
			{6, JointSetting{
				 MotorType : motor_type::PRS,
				 RealID : 2,
				 Offset : 2,
			 }},
			{7, JointSetting{
				 MotorType : motor_type::PRS,
				 RealID : 3,
				 Offset : 3,
			 }},
			{8, JointSetting{
				 MotorType : motor_type::ENR,
				 RealID : 4,
				 Offset : 0,
			 }},
			{9, JointSetting{
				 MotorType : motor_type::ENR,
				 RealID : 5,
				 Offset : 1,
			 }},
			{10, JointSetting{
				 MotorType : motor_type::ENR,
				 RealID : 6,
				 Offset : 2,
			 }},
			{11, JointSetting{
				 MotorType : motor_type::ENR,
				 RealID : 7,
				 Offset : 3,
			 }},
		},
	};
	return conf;
}