#ifndef JOINT_FACTORY_H_
#define JOINT_FACTORY_H_

#include "Joint.h"

class JointFactory {
private:
	std::vector<Joint*> joint_list;

public:
	static Joint* create();
};

#endif
