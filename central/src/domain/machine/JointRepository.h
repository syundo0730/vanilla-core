#ifndef JOINT_REPOSITORY_H_
#define JOINT_REPOSITORY_H_

#include <vector>
#include "JointAngle.h"
#include "Pose.h"
#include "Joint.h"

class JointRepository {
private:
	std::vector<Joint*> joint_list;

public:
	JointRepository();
	virtual ~JointRepository();
    void storeCurrentJointAngle(int id, JointAngle* angle);
    void storeTargetJointAngle(int id, JointAngle* angle);
    void storeCurrentPose(int id, Pose* pose);
    void storeTargetPose(int id, Pose* pose);
	void storeJoint(int id, Joint* joint);
	JointAngle* fetchCurrentJointAngle(int id);
	JointAngle* fetchTargetJointAngle(int id);
	Pose* fetchCurrentPose(int id);
	Pose* fetchTargetPose(int id);
	Joint* fetchJoint(int id);
};

#endif
