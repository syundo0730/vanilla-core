#include "Interpolator.h"
#include <iostream>

class InterpolatorImpl : public Interpolator
{
private:
    Vector3 a_0, a_1, a_2, a_3;
public:
    void initialize(Vector3 x0, Vector3 dx0, Vector3 ddx0, double Tdbl) override {
        a_0 = x0;
        a_1 = dx0;
        a_2 = 0.5 * ddx0;
        a_3 = - ddx0 / (3 * Tdbl);

		std::cout << "interpolator initialized!!" << std::endl;
		std::cout << x0 << std::endl;
		std::cout << dx0 << std::endl;
		std::cout << ddx0 << std::endl;
		std::cout << a_0 << std::endl;
		std::cout << a_1 << std::endl;
		std::cout << a_2 << std::endl;
		std::cout << a_3 << std::endl;
    }
    Vector3 getPosition(double t) override {
        auto pos = Vector3(
            interpolate(0, t),
            interpolate(1, t),
            interpolate(2, t)
        );
		std::cout << "dblSupTime: " << t << std::endl;
        std::cout << pos << std::endl;
        return pos;
    }
private:
    double interpolate(int i, double t) {
        auto t2 = t*t;
        auto t3 = t2 * t;
		std::cout << a_0[i] << ", ";
		std::cout << a_1[i] << ", ";
		std::cout << a_2[i] << ", ";
		std::cout << a_3[i] << std::endl;
        return a_0[i] + a_1[i] * t + a_2[i] * t2 + a_3[i] * t3;
    }
};

std::unique_ptr<Interpolator> Interpolator::instantiate()
{
	return std::make_unique<InterpolatorImpl>();
}