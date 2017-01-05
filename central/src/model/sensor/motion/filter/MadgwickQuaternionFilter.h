#ifndef MADGWICK_QUATERNION_FILTER_H
#define MADGWICK_QUATERNION_FILTER_H

#include <Eigen/Dense>
#include <array>

class MadgwickQuaternionFilter
{
  private:
  	double deltat, zeta, beta;
  	std::array<double, 4> q;

  public:
    // MadgwickQuaternionFilter(double _deltat);
    MadgwickQuaternionFilter();
	void update(double ax, double ay, double az, double gx, double gy, double gz);
    Eigen::Quaterniond getAsQuaternion();
};

#endif // MADGWICK_QUATERNION_FILTER_H