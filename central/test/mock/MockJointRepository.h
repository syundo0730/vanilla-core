#ifndef MOCK_JOINT_REPOSITORY_H
#define MOCK_JOINT_REPOSITORY_H

#include "gmock/gmock.h"
#include "JointRepository.h"
#include "JointAngle.h"

class MockJointRepository : public JointRepository
{
public:
	MOCK_METHOD2(setCurrentJointAngle, void(int, JointAngle));
	MOCK_METHOD2(setTargetJointAngle, void(int, JointAngle));
	MOCK_METHOD1(getCurrentJointAngle, JointAngle(int));
	MOCK_METHOD1(getTargetJointAngle, JointAngle(int));
	MOCK_METHOD1(applyTargetAngle, void(bool));
	MOCK_METHOD0(enable, void());
	MOCK_METHOD0(free, void());
};

#endif