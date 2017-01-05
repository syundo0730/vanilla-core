#ifndef PWM_MOTOR_H
#define PWM_MOTOR_H

#include "conf.h"
#include <cstdint>

class PWMMotor
{
private:
	static const uint32_t PRS_SRV_MAX_DUTY = 2100;
	static const uint32_t PRS_SRV_MIN_DUTY = 900;
	static const uint32_t ENR_SRV_MAX_DUTY = 2370;
	static const uint32_t ENR_SRV_MIN_DUTY = 600;

public:
	static uint16_t deciDegreeToDuty(motor_type motorType, int16_t angle)
	{
		auto coef = motorType == motor_type::PRS
										? (PRS_SRV_MAX_DUTY - PRS_SRV_MIN_DUTY) / 1800.0
										: (ENR_SRV_MAX_DUTY - ENR_SRV_MIN_DUTY) / 1800.0;
		auto center = motorType == motor_type::PRS
											? (PRS_SRV_MAX_DUTY + PRS_SRV_MIN_DUTY) * 0.5
											: (ENR_SRV_MAX_DUTY + ENR_SRV_MIN_DUTY) * 0.5;
		return center + coef * angle;
	}
};

#endif