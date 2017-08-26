#ifndef RANGE_SENSOR_H
#define RANGE_SENSOR_H

#include <memory>
#include <cstdint>

class I2Cdev;

class RangeSensor
{
public:
  static std::unique_ptr<RangeSensor> instantiate(
      I2Cdev &i2cdev, int xshutPinNum, int intPinNum);
  virtual void activate(uint8_t address, uint16_t timeout = 500) = 0;
  virtual uint16_t readRangeAsMilliMeters() = 0;
};

#endif // RANGE_SENSOR_H