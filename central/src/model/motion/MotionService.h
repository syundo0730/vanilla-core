#ifndef MOTION_SERVICE_H
#define MOTION_SERVICE_H

#include <memory>

class MotionLoader;
class JointRepository;
class Conf;

class MotionService
{
public:
	static std::unique_ptr<MotionService> instantiate(
			MotionLoader &motionLoader,
			JointRepository &jointRepository,
			Conf &conf);
	virtual void update() = 0;
	virtual void start(int) = 0;
	virtual void stop() = 0;
	virtual void pause() = 0;
};

#endif