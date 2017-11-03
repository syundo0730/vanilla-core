#ifndef JOINT_REPOSITORY_SPY_H_
#define JOINT_REPOSITORY_SPY_H_

#include "JointRepository.h"
#include <memory>
#include <map>
#include <cstdint>

class JointRepositorySpy : public JointRepository
{
public:
	virtual std::map<int, int16_t> getJointAnglesAsDeciDegree() = 0;
	static std::unique_ptr<JointRepositorySpy> instantiate();
};

#endif
