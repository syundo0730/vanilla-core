#ifndef LIP_STATE_H
#define LIP_STATE_H
#include "dimensional_types.h"
 
struct LIPState {
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 support_position;
	LegSide leg_side;
};
 
#endif // LIP_STATE_H



