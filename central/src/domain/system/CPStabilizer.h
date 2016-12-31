#ifndef CP_STABILIZER_H
#define CP_STABILIZER_H
#include "Eigen/Dense"
 
class CPStabilizer
{
private:
	double omega;
	double time_shift;
public:
	CPStabilizer(double _omega, double _time_shift);
	double stabilize(const Eigen::Vector2d &state_d, const Eigen::Vector2d &state);
};
 
#endif // CP_STABILIZER_H



