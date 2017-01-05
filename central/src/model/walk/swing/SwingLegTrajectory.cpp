#include "SwingLegTrajectory.h"
#include "conf.h"
#include <cmath>
#include <iostream>

class SwingLegTrajectoryImpl : public SwingLegTrajectory
{
private:
	double dt;
	double Tsup;
	double height;
	Vector3 startEdge;

public:
	SwingLegTrajectoryImpl(Conf &conf)
			: dt(conf.System.IntervalSec)
	{
	}
	void initialize(double _Tsup, double _height, const Vector3 &_startEdge) override
	{
		Tsup = _Tsup;
		height = _height;
		startEdge = _startEdge;
		std::cout << "initialized!!" << std::endl;
		std::cout << startEdge << std::endl;
	}
	Vector3 generate(const Vector3 &target, double t) override
	{
		Vector3 pos = startEdge + (target - startEdge) * t / Tsup;
		pos[2] = height * sin(t * M_PI / Tsup);
		return pos;
	}
};

std::unique_ptr<SwingLegTrajectory>
SwingLegTrajectory::instantiate(Conf &conf)
{
	return std::make_unique<SwingLegTrajectoryImpl>(conf);
}