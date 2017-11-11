#ifndef WALK_STATE_H
#define WALK_STATE_H
#include "dimensional_types.h"
#include "LegSide.h"
#include "COGState.h"
 
struct WalkState {
	Vector3 leftLegPosition;
	Vector3 rightLegPosition;
	LegSide supportingLegSide;
	COGState cog;
};
 
#endif // WALK_STATE_H