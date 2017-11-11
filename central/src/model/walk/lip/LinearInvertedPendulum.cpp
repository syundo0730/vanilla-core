#include "LinearInvertedPendulum.h"
#include "Gait.h"
#include "conf.h"
#include <iostream>
#include <ostream>

class LinearInvertedPendulumImpl : public LinearInvertedPendulum
{
  private:
	double Tsup;
	double Zc;
	double omega;
	double Tc;
	double C;
	double S;
	int nSteps;
	Vector2IntMap targetLegPos;
	Vector2IntMap xi, vi;

  public:
	LinearInvertedPendulumImpl(Conf &conf)
		: nSteps(conf.Walk.PredictionStepNum)
	{
	}
	void setParams(double _Tsup, double _Zc) override
	{
		Tsup = _Tsup;
		Zc = _Zc;
		using Physic::g;
		omega = Zc / g;
		Tc = sqrt(omega);
		C = cosh(Tsup / Tc);
		S = sinh(Tsup / Tc);
	}
	void optimizeStep(bool isFirstStep, LegSide supportingLegSide, const Gait &gait) override
	{
		Vector2 initialLegPos, x, v;
		if (isFirstStep) {
			initialLegPos = supportingLegSide == LegSide::RIGHT
							 ? Vector2(0, -0.5 * gait.stepAmount[1])
							 : Vector2(0, 0.5 * gait.stepAmount[1]);
			x = Vector2(0, 0);
			v = calcTargetVelocity(initialLegPos);
		} else {
			initialLegPos = targetLegPos[1];
			x = xi[1];
			v = vi[1];
		}
		// std::cout << "x: " << x << std::endl;
		// std::cout << "v: " << x << std::endl;
		// std::cout << "initialLegPos: " << initialLegPos << std::endl;
		// std::cout << "gat x: " << gait.stepAmount[0] << " gait y: " << gait.stepAmount[1] << std::endl;
		auto walkParam = calcWalkParam(isFirstStep, gait.stepAmount[0], gait.stepAmount[1]);
		// std::cout << "walk params" << std::endl;
		// for (auto &kv : walkParam) {
		// 	auto val = kv.second;
		// 	std::cout << val << std::endl;
		// }
		optimizeTarget(x, v, initialLegPos, walkParam, supportingLegSide, 10, 1);
	}
	COGState getCOGState(double t, int n) override
	{
		auto cog = calcCog(t, n);
		return COGState{
			projectTo(cog[0], Zc), // position
			projectTo(cog[1], 0),   // velocity
			projectTo(cog[2], 0)	// acceleration
		};
	}
	Vector3 getTargetLegPos(int n) override
	{
		return projectTo(targetLegPos[n], 0);
	}

  private:
	void optimizeTarget(
		const Vector2 &xi0,
		const Vector2 &vi0,
		const Vector2 &initialLegPos,
		Vector2IntMap walkParam,
		LegSide supportingSide,
		double a,
		double b)
	{
		xi[0] = xi0;
		vi[0] = vi0;
		targetLegPos[0] = initialLegPos;

		Vector2IntMap legPos = {{0, initialLegPos}};
		const int index_shift = (supportingSide == LegSide::RIGHT) ? 0 : 1;

		std::cout << "xi0" << std::endl;
		std::cout << xi[0] << std::endl;
		std::cout << "vi0" << std::endl;
		std::cout << vi[0] << std::endl;
		std::cout << "targetpos0" << std::endl;
		std::cout << targetLegPos[0] << std::endl;

		for (int n = 1; n <= nSteps; ++n) {
			// leg position
			double sy = (n % 2 == index_shift) ? -walkParam[n][1] : walkParam[n][1];
			legPos[n][0] = legPos[n - 1][0] + walkParam[n][0];
			legPos[n][1] = legPos[n - 1][1] + sy;

			// std::cout << "legPos" << std::endl;
			// std::cout << legPos[n] << std::endl;

			// initial COG position and velocity
			xi[n] = C * xi[n - 1] + Tc * S * vi[n - 1] + (1 - C) * targetLegPos[n - 1];
			vi[n] = S * xi[n - 1] / Tc + C * vi[n - 1] - S * targetLegPos[n - 1] / Tc;

			// std::cout << "xi" << std::endl;
			// std::cout << xi[n] << std::endl;
			// std::cout << "vi" << std::endl;
			// std::cout << vi[n] << std::endl;

			// end COG position and velocity
			double x = walkParam[n + 1][0] * 0.5;
			double y = (n % 2 == index_shift) ? 0.5 * walkParam[n + 1][1] : -0.5 * walkParam[n + 1][1];
			auto p = Vector2(x, y);
			auto xe = legPos[n] + p;
			auto ve = calcTargetVelocity(p);

			// std::cout << "xe" << std::endl;
			// std::cout << xe << std::endl;
			// std::cout << "ve" << std::endl;
			// std::cout << ve << std::endl;

			// optimal leg positoins
			double D = a * (C - 1) * (C - 1) + b * (S / Tc) * (S / Tc);
			targetLegPos[n] = -a * (C - 1) * (xe - C * xi[n] - Tc * S * vi[n]) / D - b * S * (ve - S * xi[n] / Tc - C * vi[n]) / (Tc * D);
		}
	}
	std::array<Vector2, 3> calcCog(double t, int n)
	{
		double cosht = cosh(t / Tc);
		double sinht = sinh(t / Tc);
		auto x = (xi[n] - targetLegPos[n]) * cosht + Tc * vi[n] * sinht + targetLegPos[n];
		auto v = (xi[n] - targetLegPos[n]) / Tc * sinht + vi[n] * cosht;
		auto a = omega * x;

		std::array<Vector2, 3> cog{{x, v, a}};
		return cog;
	}
	Vector2IntMap calcWalkParam(
		bool isFirstStep,
		double targetLongitudinalStepLength,
		double targetHorizontalStepLength)
	{
		Vector2IntMap walkParam;
		// fist step
		if (isFirstStep) {
			walkParam[1] = Vector2(
				0,
				targetHorizontalStepLength);
		} else {
			walkParam[1] = Vector2(
				targetLongitudinalStepLength,
				targetHorizontalStepLength);
		}
		// 2nd step - previous step of last
		for (int i = 2; i < nSteps; ++i) {
			walkParam[i] = Vector2(
				targetLongitudinalStepLength,
				targetHorizontalStepLength);
		}
		// last step
		walkParam[nSteps] = Vector2(
			0,
			targetHorizontalStepLength);
		return walkParam;
	}
	Vector2 calcTargetVelocity(const Vector2 &p)
	{
		return Vector2((C + 1) * p[0] / (Tc * S),
					   (C - 1) * p[1] / (Tc * S));
	}
	Vector3 projectTo(Vector2 src, double z)
	{
		return Vector3(
			src[0],
			src[1],
			z
		);
	}
};

std::unique_ptr<LinearInvertedPendulum> LinearInvertedPendulum::instantiate(Conf &conf)
{
	return std::make_unique<LinearInvertedPendulumImpl>(conf);
}