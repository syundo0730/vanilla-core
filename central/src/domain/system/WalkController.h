#ifndef WALK_CONTROLLER_H
#define WALK_CONTROLLER_H

#include <iostream>
#include <fstream>
#include <random>
#include "WalkGenerator.h"
#include "CPStabilizer.h"
#include "physical_const.h"

class WalkController
{
public:
	void run() {
		const int step = 10;
		WalkGenerator<double> walkgen(step);
		walkgen.setWalkParam(1, Eigen::Vector2d(0, 0.2));
		walkgen.setWalkParam(2, Eigen::Vector2d(0.2, 0.2));
		walkgen.setWalkParam(3, Eigen::Vector2d(0.2, 0.2));
		walkgen.setWalkParam(4, Eigen::Vector2d(0.2, 0.2));
		walkgen.setWalkParam(5, Eigen::Vector2d(0.2, 0.2));
		walkgen.setWalkParam(6, Eigen::Vector2d(0.2, 0.2));
		walkgen.setWalkParam(7, Eigen::Vector2d(0.2, 0.2));
		walkgen.setWalkParam(8, Eigen::Vector2d(0.2, 0.2));
		walkgen.setWalkParam(9, Eigen::Vector2d(0.2, 0.2));
		walkgen.setWalkParam(10, Eigen::Vector2d(0, 0.2));
		const double Tc = 0.8, z_c = 0.8;
		walkgen.setInitialParam(Tc, z_c, Eigen::Vector2d(0, 0));
		walkgen.execOptimization(10, 1);
		// walkgen.writeCogToStream(std::ofstream("result.csv"));
		using Physic::g;
		const double dt = 0.1;
		const double omega = sqrt(g / z_c);
		const double time_shift = 0.2;
		CPStabilizer stablizer(omega, time_shift);
		auto ofs = std::ofstream("result.csv");

		// disturbance
		std::random_device rnd;     // 非決定的な乱数生成器でシード生成機を生成
		std::mt19937 mt(rnd()); //  メルセンヌツイスターの32ビット版、引数は初期シード
		std::normal_distribution<> norm(0, 0.01);

        for (int n = 0; n <= step; ++n) {
			const Eigen::Vector2d leg_pos = walkgen.getLegPosition(n);
            for (double t =0; t < Tc; t += dt) {
				// current
                auto cog = walkgen.calcCogTrajectory(t, n);
                auto pos = cog.first, vel = cog.second;
				auto state_x = Eigen::Vector2d(
					pos[0] + norm(mt),
					vel[0] + norm(mt));
				auto state_y = Eigen::Vector2d(
					pos[1] + norm(mt),
					vel[1] + norm(mt));

				// dist
                auto cog_d = walkgen.calcCogTrajectory(t + time_shift, n);
                auto pos_d = cog_d.first, vel_d = cog_d.second;
				auto state_x_d = Eigen::Vector2d(pos_d[0], vel_d[0]);
				auto state_y_d = Eigen::Vector2d(pos_d[1], vel_d[1]);

				// leg pos
				auto px = stablizer.stabilize(state_x_d, state_x);
				auto py = stablizer.stabilize(state_y_d, state_y);

                ofs << pos[0] << ','
                << pos[1] << ','
                << leg_pos[0] << ','
                << leg_pos[1] << ','
                << px << ','
                << py << std::endl;
            }
        }
	}
};
 
#endif // WALK_CONTROLLER_H