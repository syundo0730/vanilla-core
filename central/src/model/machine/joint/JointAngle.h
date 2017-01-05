#ifndef JOINT_ANGLE_H_
#define JOINT_ANGLE_H_

#include <cstdint>

class JointAngle
{
  private:
	int16_t angleDeciDegree;

  public:
	JointAngle() = default;
	JointAngle(int16_t _angleDeciDegree);

  public:
	int16_t getAsDeciDegree();
	void setAsDeciDegree(int16_t _angleDeciDegree);
};

#endif
