#ifndef MOCK_RANGE_SENSOR_ARRAY_H
#define MOCK_RANGE_SENSOR_ARRAY_H

#include "gmock/gmock.h"
#include "RangeSensorArray.h"
#include <vector>
#include <cstdint>

class MockRangeSensorArray : public RangeSensorArray
{
  public:
    MOCK_METHOD0(readRanges, std::vector<uint16_t>(void));
};

#endif