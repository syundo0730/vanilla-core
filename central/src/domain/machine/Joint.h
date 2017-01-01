#ifndef JOINT_H_
#define JOINT_H_

#include "JointAngle.h"

class Joint {

private:
	uint8_t id;
	JointAngle* target_angle;
	JointAngle* current_angle;

public:
	Joint(uint8_t id, JointAngle* _target_angle, JointAngle* _current_angle):
    id_(id), target_angle(_target_angle), current_angle(_current_angle) {}

public:
	uint8_t getId() {
		return id;
	}

	JointAngle* getTargetAngle() {
		return target_angle;
	}

	JointAngle* getCurrentAngle() {
		return current_angle;
	}

	void setTargetAngle(JointAngle* angle) {
		return target_angle = angle;
	}

	void setCurrentAngle(JointAngle* angle) {
		return current_angle = angle;
	}
};

#endif
