#ifndef RANGE_SENSOR_ARRAY_H
#define RANGE_SENSOR_ARRAY_H

#include <memory>
#include <cstdint>
#include <vector>

class I2Cdev;

class RangeSensorArray
{
  public:
    static std::unique_ptr<RangeSensorArray> instantiate(I2Cdev& i2cdev);
    virtual std::vector<uint16_t> readRanges() = 0;
};

#endif // RANGE_SENSOR_ARRAY_H