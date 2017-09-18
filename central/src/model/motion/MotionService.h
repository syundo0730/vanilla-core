#ifndef MOTION_SERVICE_H
#define MOTION_SERVICE_H

#include <memory>
#include <map>

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
    virtual void setTargetJointAngle(int, int16_t) = 0;
    virtual std::map<int, int16_t> getCurrentJointAngles() = 0;
};

#endif