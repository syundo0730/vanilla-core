#include "WalkGenerator.h"
#include "dimensional_types.h"
#include "physical_const.h"
#include "LegSide.h"
#include "LinearInvertedPendulum.h"
#include "SwingLegTrajectory.h"
#include "CPStabilizer.h"
#include "Interpolator.h"
#include "conf.h"
#include <iostream>
#include <fstream>
#include <random>
#include <tuple>

class WalkGeneratorImpl : public WalkGenerator
{
  private:
	LinearInvertedPendulum &linearInvertedPendulum;
	SwingLegTrajectory &swingLegTrajectory;
	CPStabilizer &stablizer;
	Interpolator &interpolator;
	double time, dt, Tsup, Tdbl;
	double Zc;
	const int predictionStepNum;
	bool isFirstStep = true, willStop = false, isWalking = false;
	Gait currentGait;
	int remainingStep = 0;
	LegSide supportingSide;

  public:
	WalkGeneratorImpl(
		LinearInvertedPendulum &linearInvertedPendulum,
		SwingLegTrajectory &swingLegTrajectory,
		CPStabilizer &stablizer,
		Interpolator &interpolator,
		Conf &conf)
		: linearInvertedPendulum(linearInvertedPendulum),
		  swingLegTrajectory(swingLegTrajectory),
		  stablizer(stablizer),
		  interpolator(interpolator),
		  time(0),
		  dt(conf.System.IntervalSec),
		  Tsup(conf.Walk.DefaultTsup),
		  Tdbl(0.1),
		  Zc(conf.Walk.DefaultZc),
		  predictionStepNum(conf.Walk.PredictionStepNum),
		  currentGait({Vector2(conf.Walk.DefaultTargetLongitudinalStepLength,
						conf.Walk.DefaultTargetHorizontalStepLength),
				0}){
				linearInvertedPendulum.setParams(Tsup, Zc);
	}
	void start() override
	{
		if (willStop) {
			return;
		}
		reset();
		isWalking = true;
	}
	void stop() override
	{
		willStop = true;
	}
	void update(Gait gait) override
	{
		currentGait = gait;
	}
	void update() override
	{
		if (!isWalking) {
			return;
		}
		if (time > Tsup)
		{
			time = 0;
			if (isFirstStep) {
				isFirstStep = false;
			}
			exhangeSupportLegSide();
		}
		if (time == 0)
		{
			if (willStop)
			{
				remainingStep--;
			} else {
				linearInvertedPendulum.optimizeStep(
					isFirstStep,
					supportingSide,
					currentGait);
			}
		}
		if (remainingStep <= 0)
		{
			onStop();
			return;
		}
		time += dt;
	}
	Gait getGait() override
	{
		return currentGait;
	}
	bool needsUpdate() override
	{
		return isWalking;
	}
  	WalkState getState() {
		int index = predictionStepNum - remainingStep;
		auto cog = linearInvertedPendulum.getCOGState(time, index);
		auto legPos = linearInvertedPendulum.getTargetLegPos(index);

		return WalkState{
			leftLegPosition : legPos,
			rightLegPosition : legPos,
			supportingLegSide: supportingSide,
			cog: cog
		};
	}
  private:
	void reset()
	{
		isWalking = false;
		isFirstStep = true;
		willStop = false;
		time = 0;
		supportingSide = LegSide::RIGHT;
		remainingStep = predictionStepNum;
	}
	void onStop() {
		reset();
	}
	void exhangeSupportLegSide() {
		supportingSide = supportingSide == LegSide::RIGHT ? LegSide::LEFT : LegSide::RIGHT;
	}
};

// WalkState WalkGenerator::stabilize(double t, int n) {
//     auto cog = linearInvertedPendulum.calcCogTrajectory(t, n);
//     auto cog_d = linearInvertedPendulum.calcCogTrajectory(t + cp_time_shift, n);
// 	auto current_state = std::pair<Vector2, Vector2>(
// 		Vector2(cog.first[0], cog.second[0]),
// 		Vector2(cog.first[1], cog.second[1])
// 	);
// 	auto target_state = std::pair<Vector2, Vector2>(
// 		Vector2(cog_d.first[0], cog_d.second[0]),
// 		Vector2(cog_d.first[1], cog_d.second[1])
// 	);
// 	auto p = Vector2(
// 		stablizer.stabilize(target_state.first, current_state.first),
// 		stablizer.stabilize(target_state.second, current_state.second)
// 	);

// 	return WalkState(p, cog.first, cog.second);
// }

std::unique_ptr<WalkGenerator> WalkGenerator::instantiate(
	LinearInvertedPendulum &linearInvertedPendulum,
	SwingLegTrajectory &swingLegTrajectory,
	CPStabilizer &stablizer,
	Interpolator &interpolator,
	Conf &conf)
{
	return std::make_unique<WalkGeneratorImpl>(
		linearInvertedPendulum,
		swingLegTrajectory,
		stablizer,
		interpolator,
		conf);
}