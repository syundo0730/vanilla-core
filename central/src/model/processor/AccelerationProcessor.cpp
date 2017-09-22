#include "AccelerationProcessor.h"
#include <cmath>

class AccelerationProcessorImpl : public AccelerationProcessor
{
  private:
	using t_falling_angle = std::array<double, 2>;
	const double FALLING_THREASHOLD = 0.8;
	const double FALL_THREASHOLD = 1.5;

  public:
	FallState process(const t_acceleration &accel) override
	{
		auto angles = _calc_angles(accel);
		auto angleX = angles[0];
		auto angleY = angles[1];

		double absAngleX = std::abs(angleX);
		double absAngleY = std::abs(angleY);
		if (absAngleX > absAngleY) {
			if (absAngleX > FALL_THREASHOLD) {
				if (angleX > 0) {
					return FallState::FallBackword;
				} else {
					return FallState::FallForward;
				}
			} else if (absAngleX > FALLING_THREASHOLD) {
				if (angleX > 0) {
					return FallState::FallingBackword;
				} else {
					return FallState::FallingForward;
				}
			}
		} else {
			if (absAngleY > FALL_THREASHOLD) {
				if (angleY > 0) {
					return FallState::FallRight;
				} else {
					return FallState::FallLeft;
				}
			} else if (absAngleX > FALLING_THREASHOLD) {
				if (angleX > 0) {
					return FallState::FallingRight;
				} else {
					return FallState::FallingLeft;
				}
			}
		}
		return FallState::Stand;
	}

  private:
	t_falling_angle _calc_angles(const t_acceleration &accel)
	{
		auto x = accel[0];
		auto y = accel[1];
		auto z = accel[2];
		double x2 = x * x;
		double y2 = y * y;
		double z2 = z * z;
		auto angleX = x / std::sqrt(y2 + z2);
		auto angleY = y / std::sqrt(x2 + z2);

		return t_falling_angle{
			angleX,
			angleY,
		};
	}
};

std::unique_ptr<AccelerationProcessor> AccelerationProcessor::instantiate()
{
	return std::make_unique<AccelerationProcessorImpl>();
}