#include "LegKinematics.h"
#include "conf.h"

class LegKinematicsImpl : public LegKinematics
{
  private:
	Vector3 root;
	double L1, L2;

  public:
	LegKinematicsImpl(const Vector3 &root, Conf &conf)
		: root(root),
		  L1(conf.LegKinematics.L1),
		  L2(conf.LegKinematics.L2)
	{
	}
	std::array<double, 6> inverse(const Vector3 &target) override
	{
		auto localTarget = target - root;
		double x = localTarget[0];
		double y = localTarget[1];
		double z = localTarget[2];

		double L = sqrt(z * z + y * y) - 2 * L1;
		double h = sqrt(x * x + L * L);

		double a = atan(y / z);
		double b = acos(h / (2 * L2));
		double g = asin(x / h);

		std::array<double, 6>
			angle{{0,
				   a,
				   (b - g),
				   2 * b,
				   (b + g),
				   a}};

		return angle;
	}
	Vector3 forward(std::vector<uint16_t> angles) override
	{
		return Vector3(0, 0, 0);
	}
};

std::unique_ptr<LegKinematics> LegKinematics::instantiate(
	const Vector3 &root, Conf &conf)
{
	return std::make_unique<LegKinematicsImpl>(root, conf);
}