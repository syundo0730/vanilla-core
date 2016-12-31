#ifndef WALK_GENERATOR_H
#define WALK_GENERATOR_H
#include <iostream>
#include <ostream>
#include <map>
#include "Eigen/Dense"
#include "physical_const.h"
 
template <class T>
class WalkGenerator
{
 
private:
    typedef std::map<int, Eigen::Vector2d, std::less<int>, Eigen::aligned_allocator<std::pair<const int, Eigen::Vector2d> > > Vector2dMap;
    T Tsup;
    T z_c;
    T Tc;
    T C;
    T S;
    T g;
    int n_steps;
    Vector2dMap walk_param;
    Vector2dMap leg_param;
    Vector2dMap target_x, target_v;
    Vector2dMap target_x_leg;
    Vector2dMap xi, vi;
 
    Vector2dMap pf_n;
    Vector2dMap vf_n;
 
public:
    WalkGenerator(int n) : n_steps(n) {}
 
public:
    /**
     * 定数の初期化
    **/
    void setInitialParam(T _Tsup, T _z_c, const Eigen::Vector2d &xi0) {
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

    Eigen::Vector2d getLegPosition(int n) {
        return target_x_leg[n];
    }
 
    /**
     * 歩行素片の設定
    **/
    void setWalkParam(int n, const Eigen::Vector2d &param) {
        walk_param[n] = param;
    }
 
    void execOptimization(T a, T b) {
        initLegParam(Eigen::Vector2d(0, -0.1));
        initTarget();
        initTargetLeg(a, b);
    }

    void writeCogToStream(std::ostream &&ost) {
        for (int n = 0; n <= 5; ++n) {
            auto leg_pos = target_x_leg[n];
            for (double t =0; t < 0.8; t += 0.1) {
                auto ret = calcCogTrajectory(t, n);
                auto pos = ret.first;
                ost << pos[0] << ','
                << pos[1] << ','
                << leg_pos[0] << ','
                << leg_pos[1] << std::endl;
            }
        }
    }
    //重心位置の解析解(p.130)
    std::pair<Eigen::Vector2d, Eigen::Vector2d> calcCogTrajectory(T t, int n) {
        T cosht = cosh(t/Tc);
        T sinht = sinh(t/Tc);
        return std::pair<Eigen::Vector2d, Eigen::Vector2d> (
                    (xi[n] - target_x_leg[n])*cosht + Tc*vi[n]*sinht + target_x_leg[n],
                    (xi[n] - target_x_leg[n])/Tc*sinht + vi[n]*cosht
        );
    }
 
private:
    //歩行パラメタから求まる着地位置(p.128)
    void initLegParam(const Eigen::Vector2d &inilegp){
        leg_param[0] = inilegp;
        for (int n = 1; n <= n_steps; ++n) {
            auto sy = (n%2 == 0) ? -walk_param[n][1] : walk_param[n][1];
            leg_param[n][0] = leg_param[n-1][0] + walk_param[n][0];
            leg_param[n][1] = leg_param[n-1][1] + sy;
        }
    }
    //歩行素片のパラメータ(p.129, 131)
    void initTarget() {
        for (int n = 0; n <= n_steps; ++n) {
            T x = walk_param[n+1][0] * 0.5;
            T y = (n % 2 == 0) ? 0.5 * walk_param[n+1][1] : -0.5 * walk_param[n+1][1];
            auto p = Eigen::Vector2d(x, y);
            target_x[n] = leg_param[n] + p;
            target_v[n] = calcTargetVelocity(p);
        }
    }
    //歩行素片から終端位置，速度を求める
    Eigen::Vector2d calcTargetVelocity(const Eigen::Vector2d &p) {
        return Eigen::Vector2d ( (C + 1) * p[0] / (Tc*S),
                (C - 1) * p[1] / (Tc*S) );
    }
 
    //最適解を得る
    void initTargetLeg(T a, T b) {
        target_x_leg[0] = leg_param[0];
        for (int n = 1; n <= n_steps; ++n) {
            //初速度(p.130)
            xi[n] = C*xi[n-1] + Tc*S*vi[n-1] + (1-C)*target_x_leg[n-1];
            vi[n] = S*xi[n-1]/Tc + C*vi[n-1] - S*target_x_leg[n-1]/Tc;
            //最適解(p.131)
            T D = a*(C - 1)*(C - 1) + b*(S/Tc)*(S/Tc);
            target_x_leg[n] = -a*(C-1)*(target_x[n] - C*xi[n] - Tc*S*vi[n])/D
                    - b*S*(target_v[n] - S*xi[n]/Tc - C*vi[n])/(Tc*D);
        }
    }
};
 
#endif // WALK_GENERATOR_H