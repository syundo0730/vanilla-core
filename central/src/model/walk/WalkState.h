#ifndef WALK_STATE_H
#define WALK_STATE_H
#include "dimensional_types.h"
#include "LegSide.h"
 
struct WalkState {
	Vector3 left_leg_position;
	Vector3 right_leg_position;
	LegSide supporting_leg_side;
	Vector3 body_position;
	Vector3 body_velocity;
	Vector3 body_acceleration;
};
 
#endif // WALK_STATE_H