#ifndef MOCK_CP_STABILIZER_H
#define MOCK_CP_STABILIZER_H

#include "gmock/gmock.h"
#include "CPStabilizer.h"
#include "dimensional_types.h"

class MockCPStabilizer : public CPStabilizer
{
public:
  MOCK_METHOD2(stabilize, double(const Vector2 &state_d, const Vector2 &state));
};
#endif