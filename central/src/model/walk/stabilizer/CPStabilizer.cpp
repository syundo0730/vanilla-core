#include "CPStabilizer.h"
#include "physical_const.h"
#include "conf.h"
#include <cmath>

class CPStabilizerImpl : public CPStabilizer
{
  private:
	double omega;
	double time_shift;

  public:
		CPStabilizerImpl(Conf &conf)
				: omega(conf.CPStabilizer.Omega),
					time_shift(conf.CPStabilizer.TimeShift)
		{
		}
		double stabilize(const Vector2 &state_d, const Vector2 &state) override
		{
			const double xi_d = state_d[0] + state_d[1] / omega;
			const double xi = state[0] + state[1] / omega;
			using Physic::e;
			const double ek = std::pow(e, omega * time_shift);
			return (xi_d - xi * ek) / (1 - ek);
	}
};

std::unique_ptr<CPStabilizer> CPStabilizer::instantiate(Conf &conf)
{
	return std::make_unique<CPStabilizerImpl>(conf);
}