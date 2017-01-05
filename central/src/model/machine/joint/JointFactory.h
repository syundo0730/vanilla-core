#ifndef JOINT_FACTORY_H_
#define JOINT_FACTORY_H_

#include "Joint.h"

class JointFactory {

public:
	static Joint* create();
};

#endif
