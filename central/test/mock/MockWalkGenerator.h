#ifndef MOCK_WALK_GENERATOR_H
#define MOCK_WALK_GENERATOR_H

#include "gmock/gmock.h"
#include "WalkGenerator.h"
#include "WalkState.h"
#include "Gait.h"

class MockWalkGenerator : public WalkGenerator
{
  public:
    MOCK_METHOD0(update, void(void));
    MOCK_METHOD1(update, void(Gait));
    MOCK_METHOD0(start, void(void));
    MOCK_METHOD0(stop, void(void));
    MOCK_METHOD0(getState, WalkState(void));
    MOCK_METHOD0(getGait, Gait(void));
    MOCK_METHOD0(needsUpdate, bool(void));
};

#endif