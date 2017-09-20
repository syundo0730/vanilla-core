#ifndef MOCK_MOTION_SERVICE_H
#define MOCK_MOTION_SERVICE_H

#include "gmock/gmock.h"
#include "MotionService.h"
#include <cstdint>
#include <map>

class MockMotionService : public MotionService
{
  public:
    MOCK_METHOD0(reloadMotion, void(void));
    MOCK_METHOD0(update, void(void));
    MOCK_METHOD1(start, void(int));
    MOCK_METHOD0(stop, void(void));
    MOCK_METHOD0(pause, void(void));
    MOCK_METHOD2(setTargetJointAngle, void(int, int16_t));
    MOCK_METHOD0(getCurrentJointAngles, std::map<int, int16_t>(void));
};

#endif