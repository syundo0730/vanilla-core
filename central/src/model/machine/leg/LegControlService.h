#ifndef LEG_CONTROL_SERVICE_H_
#define LEG_CONTROL_SERVICE_H_

#include <memory>
#include "dimensional_types.h"

class JointRepository;
class LegKinematics;
class Conf;

class LegControlService {
public:
	static std::unique_ptr<LegControlService> instantiate(
			JointRepository &jointRepository,
			LegKinematics &leftLegKinematics,
			LegKinematics &rightLegKinematics,
			Conf &conf);
	virtual void setEndEffectorPosition(const Vector3 &leftTarget, const Vector3 &rightTarget) = 0;
};

#endif