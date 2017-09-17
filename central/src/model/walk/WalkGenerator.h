#ifndef WALK_GENERATOR_H
#define WALK_GENERATOR_H

#include <memory>
#include "WalkState.h"
#include "Gait.h"

class LinearInvertedPendulum;
class SwingLegTrajectory;
class CPStabilizer;
class Conf;

class WalkGenerator
{
  public:
	static std::unique_ptr<WalkGenerator> instantiate(
		LinearInvertedPendulum &linear_inverted_pendulum,
		SwingLegTrajectory &swingLegTrajectory,
		CPStabilizer &stablizer,
		Conf &conf);
	virtual void update() = 0;
	virtual void update(Gait gait) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual WalkState getState() = 0;
	virtual Gait getGait() = 0;
	virtual bool needsUpdate() = 0;
};

#endif // WALK_GENERATOR_H