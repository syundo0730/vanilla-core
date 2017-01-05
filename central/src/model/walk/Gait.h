#ifndef GAIT_H
#define GAIT_H
#include "dimensional_types.h"

struct Gait {
	Vector2 step_amount;
	double direction;
};
 
#endif // GAIT_H