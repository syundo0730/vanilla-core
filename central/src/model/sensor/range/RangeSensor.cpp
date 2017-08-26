#include "RangeSensor.h"
#include "I2Cdev.h"
#include "VL53L0X.h"
#include "Timer.h"
#include "mraa.hpp"
#include <iostream>

class RangeSensorImpl : public RangeSensor
{
private:
  VL53L0X vl53l0x;
  mraa::Gpio xshutPin;
  mraa::Gpio intPin;
  bool isFirstMeasurement;

private:
  void setInitialOutput() {
    xshutPin.dir(mraa::DIR_OUT);
    xshutPin.write(0);
  }

public:
  RangeSensorImpl(I2Cdev &i2cdev, int xshutPinNum, int intPinNum)
      : vl53l0x(i2cdev), xshutPin(xshutPinNum), intPin(intPinNum), isFirstMeasurement(true)
  {
    setInitialOutput();
  }
  ~RangeSensorImpl() {
    setInitialOutput();
  }

  void activate(uint8_t address, uint16_t timeout) override
  {
    xshutPin.dir(mraa::DIR_IN);
    std::cout << "activating ...";
    timer::waitMs(150);
    std::cout << "...";
    vl53l0x.init(true);
    vl53l0x.setTimeout(timeout);
    std::cout << "...";
    timer::waitMs(100);
    vl53l0x.setAddress(address);
    std::cout << std::endl;
  }

  uint16_t readRangeAsMilliMeters() override
  {
    if (isFirstMeasurement) {
      vl53l0x.startContinuous();
      isFirstMeasurement = false;
    }
    auto range = vl53l0x.readRangeContinuousMillimeters();
    if (vl53l0x.timeoutOccurred())
    {
      return 0;
    }
    return range;
  }
};

std::unique_ptr<RangeSensor> RangeSensor::instantiate(
    I2Cdev &i2cdev, int xshutPinNum, int intPinNum)
{
  return std::make_unique<RangeSensorImpl>(i2cdev, xshutPinNum, intPinNum);
}