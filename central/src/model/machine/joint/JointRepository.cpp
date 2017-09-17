#include "JointRepository.h"
#include "JointAngle.h"
#include "RSApi.h"
#include "Adafruit_PWMServoDriver.h"
#include "Joint.h"
#include "conf.h"
#include "PWMMotor.h"
#include <map>
#include <iostream>

class JointRepositoryImpl : public JointRepository
{
  private:
	std::map<int, Joint> jointMap;
	RSApi &rsapi;
	Adafruit_PWMServoDriver &pwmapi;
	std::map<int, JointSetting> &settingMap;

  public:
	JointRepositoryImpl(
		RSApi &rsapi,
		Adafruit_PWMServoDriver &pwmapi,
		Conf &conf)
		: rsapi(rsapi),
		  pwmapi(pwmapi),
		  settingMap(conf.Joint.SettingMap)
	{
		resetTargetJointAngles();
		// power on rs servo motors
		rsapi.on();
		// start pwm
		pwmapi.begin();
		pwmapi.setPrescale(64); //This value is decided for 10ms interval.
	}

	void setCurrentJointAngle(int id, JointAngle angle) override
	{
		std::cout << "setCurrentJointAngle: " << id << " " << angle.getAsDeciDegree() << std::endl;
		jointMap[id].setCurrentAngle(angle);
	}
	void setTargetJointAngle(int id, JointAngle angle) override
	{
		std::cout << "setTargetJointAngle: " << id << " " << angle.getAsDeciDegree() << std::endl;
		jointMap[id].setTargetAngle(angle);
	}
	JointAngle getCurrentJointAngle(int id) override
	{
		return jointMap[id].getCurrentAngle();
	}
	JointAngle getTargetJointAngle(int id) override
	{
		return jointMap[id].getTargetAngle();
	}
	void applyTargetAngle() override
	{
		if (!needsApplication()) {
			return;
		}
		std::vector<uint8_t> rsIDs;
		std::vector<uint16_t> rsPositions;
		std::cout << "apply: ";
		for (const auto &kv : settingMap)
		{
			auto jointID = kv.first;
			auto s = kv.second;
			auto realID = s.RealID;
			auto target = jointMap[jointID].getTargetAngle().getAsDeciDegree();
			auto a = target + s.Offset;
			std::cout << target << ", " << std::endl;
			if (s.MotorType == motor_type::RS) {
				rsIDs.push_back(realID);
				rsPositions.push_back(a);
			} else {
				pwmapi.setDuty(realID, PWMMotor::deciDegreeToDuty(s.MotorType, a));
			}

			// TODO: current angle is asumed to be same as target angle for now
			// we should implement real angle feedback
			setCurrentJointAngle(jointID, target);
		}
		std::cout << std::endl;
		rsapi.sendMultiPosition(rsIDs, rsPositions);
		start_motion:0
	}

  private:
	void resetTargetJointAngles()
	{
		for (const auto &kv : settingMap)
		{
			auto id = kv.first;
			auto angle = JointAngle(0);
			setTargetJointAngle(id, angle);
			setCurrentJointAngle(id, angle);
		}
	}
	bool needsApplication() {
		for (const auto &kv : settingMap)
		{
			auto id = kv.first;
			auto current = getCurrentJointAngle(id).getAsDeciDegree();
			auto target = getTargetJointAngle(id).getAsDeciDegree();
			if (current != target) {
				return true;
			}
		}
		return false;
	}
};

std::unique_ptr<JointRepository> JointRepository::instantiate(
	RSApi &rsApi,
	Adafruit_PWMServoDriver &adafruit_PWMServoDriver,
	Conf &conf)
{
	return std::make_unique<JointRepositoryImpl>(
		rsApi,
		adafruit_PWMServoDriver,
		conf);
}