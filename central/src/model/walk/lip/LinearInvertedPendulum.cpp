#include "LinearInvertedPendulum.h"
#include "conf.h"
#include <iostream>
#include <ostream>

class LinearInvertedPendulumImpl : public LinearInvertedPendulum
{
  private:
	double Tsup;
	double z_c;
	double omega;
	double Tc;
	double C;
	double S;
	int n_steps;
	Vector2IntMap walk_param;
	Vector2IntMap leg_param;
	Vector2IntMap target_x, target_v;
	Vector2IntMap target_x_leg;
	Vector2IntMap xi, vi;
	Vector2IntMap pf_n;
	Vector2IntMap vf_n;
	LegSide supporting_leg_side;

  public:
	LinearInvertedPendulumImpl(Conf &conf)
		: n_steps(conf.Walk.PredictionStepNum)
	{
	}
	void setInitialParam(double _Tsup, double _z_c) override
	{
		Tsup = _Tsup;
		z_c = _z_c;
		using Physic::g;
		omega = z_c / g;
		Tc = sqrt(omega);
		C = cosh(Tsup / Tc);
		S = sinh(Tsup / Tc);
	}
	void setupFirstStep(Vector2IntMap wp, LegSide _supporting_leg_side) override
	{
		walk_param = wp;
		Vector2 leg_p;
		if (_supporting_leg_side == LegSide::RIGHT)
		{
			leg_p = Vector2(0, -0.5 * wp[1][1]);
		}
		else
		{
			leg_p = Vector2(0, 0.5 * wp[1][1]);
		}
		auto v = calcTargetVelocity(leg_p);
		auto x = Vector2(0, 0);
		initLegParamAndTarget(x, v, leg_p, _supporting_leg_side);
	}
	void switchToNextStep(Vector2IntMap wp) override
	{
		walk_param = wp;
		if (supporting_leg_side == LegSide::RIGHT)
		{
			supporting_leg_side = LegSide::LEFT;
		}
		else
		{
			supporting_leg_side = LegSide::RIGHT;
		}
		initLegParamAndTarget(xi[1], vi[1], target_x_leg[1], supporting_leg_side);
	}
	void optimize(double a, double b) override
	{
		// optimize target leg position
		target_x_leg[0] = leg_param[0];
		for (int n = 1; n <= n_steps; ++n)
		{
			//初速度(p.130)
			xi[n] = C * xi[n - 1] + Tc * S * vi[n - 1] + (1 - C) * target_x_leg[n - 1];
			vi[n] = S * xi[n - 1] / Tc + C * vi[n - 1] - S * target_x_leg[n - 1] / Tc;
			//最適解(p.131)
			double D = a * (C - 1) * (C - 1) + b * (S / Tc) * (S / Tc);
			target_x_leg[n] = -a * (C - 1) * (target_x[n] - C * xi[n] - Tc * S * vi[n]) / D - b * S * (target_v[n] - S * xi[n] / Tc - C * vi[n]) / (Tc * D);
		}
	}
	LIPState getState(double t, int n) override
	{
		auto cog = calcCogTrajectory(t, n);
		auto counter_side = supporting_leg_side == LegSide::RIGHT ? LegSide::LEFT : LegSide::RIGHT;
		auto leg_side = n % 2 == 0 ? supporting_leg_side : counter_side;
		return LIPState{
			projectTo(cog[0], z_c), // position
			projectTo(cog[1], 0), // velocity
			projectTo(cog[2], 0), // acceleration
			projectTo(target_x_leg[n], 0), // leg tip position
			leg_side}; // side of supporting leg
	}

  private:
	Vector3 projectTo(Vector2 src, double z)
	{
		return Vector3(
			src[0],
			src[1],
			z
		);
	}
	void initLegParamAndTarget(
		const Vector2 &xi0,
		const Vector2 &vi0,
		const Vector2 &initial_leg_position,
		LegSide supporting_side)
	{
		// initial poistion and velocity
		xi[0] = xi0;
		vi[0] = vi0;
		leg_param[0] = initial_leg_position;
		const int index_shift = (supporting_side == LegSide::RIGHT) ? 0 : 1;
		for (int n = 1; n <= n_steps; ++n)
		{
			// leg param
			double sy = (n % 2 == index_shift) ? -walk_param[n][1] : walk_param[n][1];
			leg_param[n][0] = leg_param[n - 1][0] + walk_param[n][0];
			leg_param[n][1] = leg_param[n - 1][1] + sy;
			// leg target
			double x = walk_param[n + 1][0] * 0.5;
			double y = (n % 2 == index_shift) ? 0.5 * walk_param[n + 1][1] : -0.5 * walk_param[n + 1][1];
			auto p = Vector2(x, y);
			target_x[n] = leg_param[n] + p;
			target_v[n] = calcTargetVelocity(p);
		}
	}
	Vector2 calcTargetVelocity(const Vector2 &p)
	{
		return Vector2((C + 1) * p[0] / (Tc * S),
							   (C - 1) * p[1] / (Tc * S));
	}
	std::array<Vector2, 3> calcCogTrajectory(double t, int n)
	{
		double cosht = cosh(t / Tc);
		double sinht = sinh(t / Tc);
		auto x = (xi[n] - target_x_leg[n]) * cosht + Tc * vi[n] * sinht + target_x_leg[n];
		auto v = (xi[n] - target_x_leg[n]) / Tc * sinht + vi[n] * cosht;
		auto a = omega * x;

		std::array<Vector2, 3> cog{{x, v, a}};
		return cog;
	}
};

std::unique_ptr<LinearInvertedPendulum> LinearInvertedPendulum::instantiate(Conf &conf)
{
	return std::make_unique<LinearInvertedPendulumImpl>(conf);
}