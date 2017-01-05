#ifndef MOCK_LEG_CONTROL_SERVICE_H
#define MOCK_LEG_CONTROL_SERVICE_H

#include "gmock/gmock.h"
#include "LegControlService.h"
#include "dimensional_types.h"

class MockLegControlService : public LegControlService
{
  public:
    MOCK_METHOD2(setEndEffectorPosition, void(const Vector3 &, const Vector3 &));
};

#endif