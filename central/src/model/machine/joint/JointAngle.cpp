#include "JointAngle.h"

JointAngle::JointAngle(int16_t _angleDeciDegree)
	: angleDeciDegree(_angleDeciDegree) {}

int16_t JointAngle::getAsDeciDegree()
{
	return angleDeciDegree;
}

void JointAngle::setAsDeciDegree(int16_t _angleDeciDegree)
{
	angleDeciDegree = _angleDeciDegree;
}