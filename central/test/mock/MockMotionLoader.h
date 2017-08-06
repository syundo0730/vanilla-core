#ifndef MOCK_MOTION_LOADER_H
#define MOCK_MOTION_LOADER_H

#include "gmock/gmock.h"
#include "MotionLoader.h"
#include "Motion.h"
#include <string>

class MockMotionLoader : public MotionLoader
{
  public:
    MOCK_METHOD1(load, std::vector<Motion>(std::string));
};

#endif