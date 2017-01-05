#include "LegControlService.h"
#include "JointRepository.h"
#include "LegKinematics.h"
#include "conf.h"
#include "JointAngle.h"
#include "utility.h"
#include <array>
#include <string>
#include <cstdint>

class LegControlServiceImpl : public LegControlService
{
  private:
	JointRepository &jointRepository;
	LegKinematics &leftLegKinematics;
	LegKinematics &rightLegKinematics;
	std::array<uint8_t, 6> leftLegJointIDs;
	std::array<uint8_t, 6> rightLegJointIDs;

  public:
	LegControlServiceImpl(
		JointRepository &jointRepository,
		LegKinematics &leftLegKinematics,
		LegKinematics &rightLegKinematics,
		Conf &conf)
		: jointRepository(jointRepository),
		  leftLegKinematics(leftLegKinematics),
		  rightLegKinematics(rightLegKinematics)
	{
		std::array<std::string, 6> leftJointNames{{"ll0", "ll1", "ll2", "ll3", "ll4", "ll5"}};
		std::array<std::string, 6> rightJointNames{{"lr0", "lr1", "lr2", "lr3", "lr4", "lr5"}};
		auto jointIDMap = conf.Joint.IDMap;
		for (int i = 0; i < 6; ++i)
		{
			leftLegJointIDs[i] = jointIDMap[leftJointNames[i]];
		}
		for (int i = 0; i < 6; ++i)
		{
			rightLegJointIDs[i] = jointIDMap[rightJointNames[i]];
		}
	}

	void setEndEffectorPosition(const Vector3 &leftTarget, const Vector3 &rightTarget) override
	{
		auto leftAngles = leftLegKinematics.inverse(leftTarget);
		auto rightAngles = rightLegKinematics.inverse(rightTarget);
		for (int i = 0; i < 6; ++i)
		{
			auto leftAngleAsMillDeg = utility::radianToDeciDegree(leftAngles[i]);
			auto rightAngleAsMillDeg = utility::radianToDeciDegree(rightAngles[i]);
			jointRepository.setTargetJointAngle(
				leftLegJointIDs[i],
				JointAngle(leftAngleAsMillDeg));
			jointRepository.setTargetJointAngle(
				rightLegJointIDs[i],
				JointAngle(rightAngleAsMillDeg));
		}
	}
};

std::unique_ptr<LegControlService> LegControlService::instantiate(
	JointRepository &jointRepository,
	LegKinematics &leftLegKinematics,
	LegKinematics &rightLegKinematics,
	Conf &conf)
{
	return std::make_unique<LegControlServiceImpl>(
		jointRepository,
		leftLegKinematics,
		rightLegKinematics,
		conf);
}