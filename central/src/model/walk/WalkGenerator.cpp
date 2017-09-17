#include "WalkGenerator.h"
#include "dimensional_types.h"
#include "physical_const.h"
#include "LegSide.h"
#include "LinearInvertedPendulum.h"
#include "SwingLegTrajectory.h"
#include "CPStabilizer.h"
#include "conf.h"
#include <iostream>
#include <fstream>
#include <random>
#include <tuple>

class WalkGeneratorImpl : public WalkGenerator
{
  private:
	LinearInvertedPendulum &linear_inverted_pendulum;
	SwingLegTrajectory &swingLegTrajectory;
	CPStabilizer &stablizer;
	const double Tsup;
	double z_c;
	const int prediction_step_num;
	double dt;
	double time = 0;
	bool is_first_step = true, will_stop = false, isWalking = false;
	Gait currentGait;
	int remainingStep = 0;
	WalkState currentState;

  public:
	WalkGeneratorImpl(
		LinearInvertedPendulum &linear_inverted_pendulum,
		SwingLegTrajectory &swingLegTrajectory,
		CPStabilizer &stablizer,
		Conf &conf)
		: linear_inverted_pendulum(linear_inverted_pendulum),
		  swingLegTrajectory(swingLegTrajectory),
		  stablizer(stablizer),
		  Tsup(conf.Walk.DefaultTsup),
		  z_c(conf.Walk.DefaultZc),
		  prediction_step_num(conf.Walk.PredictionStepNum),
		  dt(conf.System.IntervalSec),
		  currentGait({Vector2(conf.Walk.DefaultTargetLongitudinalStepLength,
						conf.Walk.DefaultTargetHorizontalStepLength),
				0})
	{
		currentState = WalkState{
			left_leg_position: Vector3(0, 0.1, 0),
			right_leg_position: Vector3(0, -0.1, 0),
			supporting_leg_side: LegSide::RIGHT,
			body_position: Vector3(0, 0, z_c)
		};
	}
	void start() override
	{
		if (will_stop) {
			return;
		}
		reset();
		isWalking = true;
	}
	void stop() override
	{
		will_stop = true;
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
			if (is_first_step) {
				is_first_step = false;
			}
		}
		if (time == 0)
		{
			// 遊脚のパラメタの初期化
			swingLegTrajectory.initialize(
				Tsup,
				0.2,
				currentState.supporting_leg_side == LegSide::LEFT
					? currentState.left_leg_position
					: currentState.right_leg_position);
			if (will_stop)
			{
				remainingStep--;
			} else {
				// 1周期ごとに目標重心軌道を計算する
				generateTargetTrajectory(
					is_first_step,
					currentGait.step_amount[0],
					currentGait.step_amount[1]);
			}
		}
		if (remainingStep <= 0)
		{
			onStop();
			return;
		}
		updateState();
		time += dt;
	}
	WalkState getState() override
	{
		return currentState;
	}
	Gait getGait() override
	{
		return currentGait;
	}
	bool needsUpdate() override
	{
		return isWalking;
	}

  private:
  	void updateState() {
		int index = prediction_step_num - remainingStep;
		auto support = linear_inverted_pendulum.getState(time, index);
		auto swingTarget = linear_inverted_pendulum.getState(time, index + 1);

		auto swingLegPos = is_first_step
							   ? projectTo(swingTarget.support_position, 0)
							   : swingLegTrajectory.generate(projectTo(swingTarget.support_position, 0), time);
		auto supportLegPos = projectTo(support.support_position, 0);

		auto leftLegPos = support.leg_side == LegSide::LEFT ? supportLegPos : swingLegPos;
		auto rightLegPos = support.leg_side == LegSide::RIGHT ? supportLegPos : swingLegPos;
		auto bodyPos = projectTo(support.position, z_c);

		currentState = WalkState{
			left_leg_position : leftLegPos,
			right_leg_position : rightLegPos,
			supporting_leg_side : support.leg_side,
			body_position : bodyPos
		};
	}
	void reset()
	{
		isWalking = false;
		is_first_step = true;
		will_stop = false;
		time = 0;
		remainingStep = prediction_step_num;
	}
	void onStop() {
		reset();
	}
	Vector3 projectTo(Vector2 src, double z)
	{
		return Vector3(
			src[0],
			src[1],
			z
		);
	}
	void generateTargetTrajectory(
		bool _is_first_step,
		double _target_longitudinal_step_length,
		double _target_horizontal_step_length)
	{
		Vector2IntMap walk_param;
		// 1歩目
		if (_is_first_step)
		{
			walk_param[1] = Vector2(
				0,
				_target_horizontal_step_length);
		}
		else
		{
			walk_param[1] = Vector2(
				_target_longitudinal_step_length,
				_target_horizontal_step_length);
		}
		// 2歩目〜最終の一つ前
		for (int i = 2; i < prediction_step_num; ++i)
		{
			walk_param[i] = Vector2(
				_target_longitudinal_step_length,
				_target_horizontal_step_length);
		}
		// 最後
		walk_param[prediction_step_num] = Vector2(
			0,
			_target_horizontal_step_length);

		linear_inverted_pendulum.setInitialParam(Tsup, z_c);
		if (_is_first_step)
		{
			// 初期化
			linear_inverted_pendulum.setupFirstStep(walk_param, currentState.supporting_leg_side);
		}
		else
		{
			// 支持脚を入れ替える
			linear_inverted_pendulum.switchToNextStep(walk_param);
		}
		// 脚接地点の最適化
		linear_inverted_pendulum.optimize(10, 1);
	}
};

// WalkState WalkGenerator::stabilize(double t, int n) {
//     auto cog = linear_inverted_pendulum.calcCogTrajectory(t, n);
//     auto cog_d = linear_inverted_pendulum.calcCogTrajectory(t + cp_time_shift, n);
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
	LinearInvertedPendulum &linear_inverted_pendulum,
	SwingLegTrajectory &swingLegTrajectory,
	CPStabilizer &stablizer,
	Conf &conf)
{
	return std::make_unique<WalkGeneratorImpl>(
		linear_inverted_pendulum,
		swingLegTrajectory,
		stablizer,
		conf);
}