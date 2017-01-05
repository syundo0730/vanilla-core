#ifndef LIP_STATE_H
#define LIP_STATE_H
#include "dimensional_types.h"
 
struct LIPState {
	Vector2 position;
	Vector2 velocity;
	Vector2 support_position;
	LegSide leg_side;
};
 
#endif // LIP_STATE_H



