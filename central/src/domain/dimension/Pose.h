#ifndef POSE_H_
#define POSE_H_

#include <vector>
#include "JointAngle.h"

class Pose {

private:
	uint8_t id;
	std::vector<JointAngle*> angleList;

public:
	Pose(uint8_t _id, std::vector<JointAngle*> _angleList): id(_id), angleList(_angleList) {}

public:
	JointAngle* getAngleById(uint8_t id) {
		return angleList[id];
	}

    std::vector<JointAngle*> getAngleList() {
        return angleList;
    }
};

#endif
