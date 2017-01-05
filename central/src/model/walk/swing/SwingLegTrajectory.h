#ifndef SWING_LEG_TRAJECTORY_H
#define SWING_LEG_TRAJECTORY_H

#include "dimensional_types.h"
#include <memory>

class Conf;

class SwingLegTrajectory
{
  public:
	static std::unique_ptr<SwingLegTrajectory> instantiate(Conf &conf);
	virtual void initialize(double _Tsup, double _height, const Vector3 &_startEdge) = 0;
	virtual Vector3 generate(const Vector3 &target, double t) = 0;
};

#endif