#ifndef LINEAR_INVERTED_PENDULUM_H
#define LINEAR_INVERTED_PENDULUM_H
#include <map>
#include "Eigen/Dense"
#include "physical_const.h"
 
class LinearInvertedPendulum
{
 
private:
    typedef std::map<int, Eigen::Vector2d, std::less<int>, Eigen::aligned_allocator<std::pair<const int, Eigen::Vector2d> > > Vector2dMap;
    double Tsup;
    double z_c;
    double Tc;
    double C;
    double S;
    int n_steps;
    Vector2dMap walk_param;
    Vector2dMap leg_param;
    Vector2dMap target_x, target_v;
    Vector2dMap target_x_leg;
    Vector2dMap xi, vi;
 
    Vector2dMap pf_n;
    Vector2dMap vf_n;
 
public:
    LinearInvertedPendulum(int n);
 
public:
    /**
     * 定数の初期化
    **/
    void setInitialParam(double _Tsup, double _z_c, const Eigen::Vector2d &xi0);

    Eigen::Vector2d getLegPosition(int n);
 
    /**
     * 歩行素片の設定
    **/
    void setWalkParam(int n, const Eigen::Vector2d &param);
 
    void execOptimization(double a, double b);

    //重心位置の解析解(p.130)
    std::pair<Eigen::Vector2d, Eigen::Vector2d> calcCogTrajectory(double t, int n);
 
private:
    //歩行パラメタから求まる着地位置(p.128) の初期化
    void initLegParam(const Eigen::Vector2d &inilegp);

    //歩行素片のパラメータ(p.129, 131) の初期化
    void initTarget();

    //歩行素片から終端位置，速度を求める
    Eigen::Vector2d calcTargetVelocity(const Eigen::Vector2d &p);
 
    //最適解を得る
    void initTargetLeg(double a, double b);
};
 
#endif // LINEAR_INVERTED_PENDULUM_H