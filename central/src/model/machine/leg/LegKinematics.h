#ifndef LEG_KINEMATICS_H_
#define LEG_KINEMATICS_H_

#include <memory>
#include <vector>
#include <array>
#include "dimensional_types.h"
#include <cstdint>

class Conf;

class LegKinematics
{
  public:
	static std::unique_ptr<LegKinematics> instantiate(const Vector3 &root, Conf &conf);
	virtual std::array<double, 6> inverse(const Vector3 &target) = 0;
	virtual Vector3 forward(std::vector<uint16_t> angles) = 0;
};

#endif
