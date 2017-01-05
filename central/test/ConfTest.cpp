#include "gtest/gtest.h"
#include "conf.h"
#include <iostream>

namespace
{

class ConfTest : public ::testing::Test
{
};

TEST_F(ConfTest, initialize)
{
	auto conf = Conf("../config.yaml");
	auto defaultConf = Conf::defaultConf();

	EXPECT_EQ(defaultConf.CPStabilizer.Omega, conf.CPStabilizer.Omega);
	EXPECT_EQ(defaultConf.CPStabilizer.TimeShift, conf.CPStabilizer.TimeShift);

	EXPECT_EQ(defaultConf.LegKinematics.L1, conf.LegKinematics.L1);
	EXPECT_EQ(defaultConf.LegKinematics.L2, conf.LegKinematics.L2);
	EXPECT_EQ(defaultConf.LegKinematics.LegRootLeft, conf.LegKinematics.LegRootLeft);
	EXPECT_EQ(defaultConf.LegKinematics.LegRootRight, conf.LegKinematics.LegRootRight);

	EXPECT_EQ(defaultConf.SerialPort.PortPath, conf.SerialPort.PortPath);
	EXPECT_EQ(defaultConf.SerialPort.BaudRate, conf.SerialPort.BaudRate);

	EXPECT_EQ(defaultConf.System.IntervalSec, conf.System.IntervalSec);
	EXPECT_EQ(defaultConf.System.GAcceleration, conf.System.GAcceleration);

	EXPECT_EQ(defaultConf.Walk.DefaultZc, conf.Walk.DefaultZc);
	EXPECT_EQ(defaultConf.Walk.DefaultTsup, conf.Walk.DefaultTsup);
	EXPECT_EQ(defaultConf.Walk.PredictionStepNum, conf.Walk.PredictionStepNum);

	for (auto &kv : conf.Joint.IDMap)
	{
		auto key = kv.first;
		auto value = kv.second;
		EXPECT_EQ(defaultConf.Joint.IDMap[key], value);
	}
	for (auto &kv : conf.Joint.SettingMap)
	{
		auto key = kv.first;
		auto setting = kv.second;
		auto defaultSetting = defaultConf.Joint.SettingMap[key];
		EXPECT_EQ(defaultSetting.Offset, setting.Offset);
		EXPECT_EQ(defaultSetting.RealID, setting.RealID);
		EXPECT_EQ(defaultSetting.MotorType, setting.MotorType);
	}
}
}