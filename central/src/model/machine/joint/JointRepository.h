#ifndef JOINT_REPOSITORY_H_
#define JOINT_REPOSITORY_H_

#include <memory>

class JointAngle;
class RSApi;
class Adafruit_PWMServoDriver;
class Conf;

class JointRepository
{
  public:
	static std::unique_ptr<JointRepository> instantiate(
		RSApi &rsApi,
		Adafruit_PWMServoDriver &adafruit_PWMServoDriver,
		Conf &conf);
	virtual void setCurrentJointAngle(int id, JointAngle angle) = 0;
	virtual void setTargetJointAngle(int id, JointAngle angle) = 0;
	virtual JointAngle getCurrentJointAngle(int id) = 0;
	virtual JointAngle getTargetJointAngle(int id) = 0;
	virtual void applyTargetAngle(bool forceApply = false) = 0;
	virtual void moveToInitialPose() = 0;
};

#endif
