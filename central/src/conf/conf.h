#ifndef CONF_H_
#define CONF_H_

#include "dimensional_types.h"
#include <string>
#include <map>

struct SerialPortConf
{
	std::string PortPath;
	unsigned int BaudRate;
};

struct SystemConf
{
	double IntervalSec;
	double GAcceleration;
};

struct LegKinematicsConf
{
	double L1, L2;
	Vector3 LegRootLeft;
	Vector3 LegRootRight;
};

struct WalkConf
{
	double DefaultZc;
	double DefaultTsup;
	int PredictionStepNum;
	double DefaultTargetHorizontalStepLength;
	double DefaultTargetLongitudinalStepLength;
};

struct CPStabilizerConf
{
	double Omega, TimeShift;
};

enum class motor_type
{
	RS,
	PRS,
	ENR
};

struct JointSetting
{
	motor_type MotorType;
	int RealID;
	int Offset;
};

struct JointConf
{
	std::map<std::string, int> IDMap;
	std::map<int, JointSetting> SettingMap;
};

struct Conf
{
  public:
	SerialPortConf SerialPort;
	SystemConf System;
	LegKinematicsConf LegKinematics;
	WalkConf Walk;
	CPStabilizerConf CPStabilizer;
	JointConf Joint;

  private:
	Conf load(std::string configFilePath);

  public:
	static Conf defaultConf();
	Conf() = default;
	Conf(std::string configFilePath);
};

#endif
