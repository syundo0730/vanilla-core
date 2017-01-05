#include "gtest/gtest.h"
#include "LegKinematics.h"
#include "conf.h"
#include "dimensional_types.h"
#include <fstream>
#include <cmath>

namespace
{

class LegKinematicsTest : public ::testing::Test
{
};

TEST_F(LegKinematicsTest, inverse)
{
	auto conf = Conf::defaultConf();
	auto root = Vector3(0, 0.03, 0);
	auto legKinematics = LegKinematics::instantiate(root, conf);

	double diff = 0;
	double coe = 0.02;
	std::ofstream ofs("legKinematics.csv");
	ofs << "t,a0,a1,a2,a3,a4,a5" << std::endl;

	for (double t = 0; t < 1; t += 0.01)
	{
		diff = coe * sin(t * 2 * M_PI);
		auto target = Vector3(0, 0.03, -0.15 + diff);
		auto angles = legKinematics->inverse(target);
		ofs
		<< t << ","
		<< angles[0] << ","
		<< angles[1] << ","
		<< angles[2] << ","
		<< angles[3] << ","
		<< angles[4] << ","
		<< angles[5] << std::endl;
	}
}
}