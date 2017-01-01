#ifndef JOINT_ANGLE_H_
#define JOINT_ANGLE_H_

#include <cstdint>

class JointAngle {

private:
	uint8_t id;
	int16_t angle_milli_deg;

public:
	JointAngle(uint8_t _id, int16_t _angle_milli_deg = 0):
	id(_id), angle_milli_deg(_angle_milli_deg) {}

public:
	uint8_t getId() {
		return id;
	}

	int16_t getAsMilliDegree() {
		return angle_milli_deg;
	}

	void setAsMilliDegree(int16_t _angle_milli_deg) {
		angle_milli_deg = _angle_milli_deg;
	}
};

#endif
