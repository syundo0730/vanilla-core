#include "gtest/gtest.h"
#include "WalkGenerator.h"
#include "LinearInvertedPendulum.h"
#include "SwingLegTrajectory.h"
#include "MockCPStabilizer.h"
#include "dimensional_types.h"
#include "conf.h"
#include <fstream>

namespace
{

class WalkGeneratorTest : public ::testing::Test
{
};

TEST_F(WalkGeneratorTest, test)
{
	MockCPStabilizer mockCPStabilizer;
	auto conf = Conf::defaultConf();
	auto lip = LinearInvertedPendulum::instantiate(conf);
	auto swingTraj = SwingLegTrajectory::instantiate(conf);
	auto wg = WalkGenerator::instantiate(
		*lip,
		*swingTraj,
		mockCPStabilizer,
		conf);

	auto Tsup = conf.Walk.DefaultTsup;
	auto dt = conf.System.IntervalSec;
	auto unitCount = Tsup / dt;
	auto stepNum = unitCount * 10;

	std::ofstream ofs("data.csv");
	ofs << "llx,lly,llz,rlx,rly,rlz,gx,gy,gz" << std::endl;

	wg->start();
	wg->update(Gait{Vector2(0.1, 0.2), 0});
	for (auto i = 0; i < stepNum; ++i)
	{
		if (i == unitCount * 3) {
			wg->update(Gait{Vector2(0.2, 0.2), 0});
		}
		if (i == unitCount * 5) {
			wg->update(Gait{Vector2(0.1, 0.2), 0});
		}
		if (i == unitCount * 6) {
			wg->stop();
		}
		wg->update();
		auto state = wg->getState();
		auto llx = state.left_leg_position[0];
		auto lly = state.left_leg_position[1];
		auto llz = state.left_leg_position[2];
		auto rlx = state.right_leg_position[0];
		auto rly = state.right_leg_position[1];
		auto rlz = state.right_leg_position[2];
		auto gx = state.body_position[0];
		auto gy = state.body_position[1];
		auto gz = state.body_position[2];

		ofs
		<< llx << ","
		<< lly << ","
		<< llz << ","
		<< rlx << ","
		<< rly << ","
		<< rlz << ","
		<< gx << ","
		<< gy << ","
		<< gz << std::endl;
	}
}
}