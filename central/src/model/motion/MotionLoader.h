#ifndef MOTION_LOADER_H
#define MOTION_LOADER_H

#include <memory>
#include <vector>
#include "Motion.h"

class MotionLoader {
  public:
	static std::unique_ptr<MotionLoader> instantiate();
	virtual std::vector<Motion> load(std::string path) = 0;
};

#endif