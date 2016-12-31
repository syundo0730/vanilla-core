#include "CPStabilizer.h"
#include "physical_const.h"
#include <cmath>

CPStabilizer::CPStabilizer(double _omega, double _time_shift) : omega(_omega), time_shift(_time_shift) {}

double CPStabilizer::stabilize(const Eigen::Vector2d &state_d, const Eigen::Vector2d &state) {
	const double xi_d = state_d[0] + state_d[1] / omega;
	const double xi = state[0] + state[1] / omega;
	using Physic::e;
	const double ek = std::pow(e, omega * time_shift);
	return (xi_d - xi * ek) / (1 - ek);
}