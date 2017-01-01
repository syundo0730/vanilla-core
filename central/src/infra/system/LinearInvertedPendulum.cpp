#include <iostream>
#include <ostream>
#include "LinearInvertedPendulum.h"
 
LinearInvertedPendulum::LinearInvertedPendulum(int n) : n_steps(n) {}

void LinearInvertedPendulum::setInitialParam(double _Tsup, double _z_c, const Eigen::Vector2d &xi0) {
	Tsup = _Tsup;
	z_c = _z_c;
	using Physic::g;
	Tc = sqrt(z_c / g);
	C = cosh(Tsup / Tc);
	S = sinh(Tsup / Tc);
	// initial poistion and velocity
	xi[0] = xi0;
	vi[0] = calcTargetVelocity(
		Eigen::Vector2d(
			0.5 * walk_param[1][0],
			-0.5 * walk_param[1][1]
		)
	);
}

Eigen::Vector2d LinearInvertedPendulum::getLegPosition(int n) {
	return target_x_leg[n];
}

void LinearInvertedPendulum::setWalkParam(int n, const Eigen::Vector2d &param) {
	walk_param[n] = param;
}

void LinearInvertedPendulum::execOptimization(double a, double b) {
	initLegParam(Eigen::Vector2d(0, -0.1));
	initTarget();
	initTargetLeg(a, b);
}

std::pair<Eigen::Vector2d, Eigen::Vector2d> LinearInvertedPendulum::calcCogTrajectory(double t, int n) {
	double cosht = cosh(t/Tc);
	double sinht = sinh(t/Tc);
	return std::pair<Eigen::Vector2d, Eigen::Vector2d> (
		(xi[n] - target_x_leg[n])*cosht + Tc*vi[n]*sinht + target_x_leg[n],
		(xi[n] - target_x_leg[n])/Tc*sinht + vi[n]*cosht
	);
}

void LinearInvertedPendulum::initLegParam(const Eigen::Vector2d &inilegp){
	leg_param[0] = inilegp;
	for (int n = 1; n <= n_steps; ++n) {
		double sy = (n%2 == 0) ? -walk_param[n][1] : walk_param[n][1];
		leg_param[n][0] = leg_param[n-1][0] + walk_param[n][0];
		leg_param[n][1] = leg_param[n-1][1] + sy;
	}
}

void LinearInvertedPendulum::initTarget() {
	for (int n = 0; n <= n_steps; ++n) {
		double x = walk_param[n+1][0] * 0.5;
		double y = (n % 2 == 0) ? 0.5 * walk_param[n+1][1] : -0.5 * walk_param[n+1][1];
		auto p = Eigen::Vector2d(x, y);
		target_x[n] = leg_param[n] + p;
		target_v[n] = calcTargetVelocity(p);
	}
}

Eigen::Vector2d LinearInvertedPendulum::calcTargetVelocity(const Eigen::Vector2d &p) {
	return Eigen::Vector2d ( (C + 1) * p[0] / (Tc*S),
			(C - 1) * p[1] / (Tc*S) );
}

void LinearInvertedPendulum::initTargetLeg(double a, double b) {
	target_x_leg[0] = leg_param[0];
	for (int n = 1; n <= n_steps; ++n) {
		//初速度(p.130)
		xi[n] = C*xi[n-1] + Tc*S*vi[n-1] + (1-C)*target_x_leg[n-1];
		vi[n] = S*xi[n-1]/Tc + C*vi[n-1] - S*target_x_leg[n-1]/Tc;
		//最適解(p.131)
		double D = a*(C - 1)*(C - 1) + b*(S/Tc)*(S/Tc);
		target_x_leg[n] = -a*(C-1)*(target_x[n] - C*xi[n] - Tc*S*vi[n])/D
				- b*S*(target_v[n] - S*xi[n]/Tc - C*vi[n])/(Tc*D);
	}
}