#include "RangeSensorArray.h"
#include "RangeSensor.h"
#include "I2Cdev.h"
#include "Timer.h"
#include "mraa.hpp"
#include <iostream>
#include <cstdint>

class RangeSensorArrayImpl : public RangeSensorArray
{
  private:
    I2Cdev &i2cdev;
    struct sensorSetting
    {
        int xshutPinNum, intPinNum;
        uint8_t address;
    };
    std::array<sensorSetting, 4 > sensorSettings;

    using t_range_sensors = std::array<std::unique_ptr<RangeSensor>, 4>;
    t_range_sensors rangeSensors;

  public:
    RangeSensorArrayImpl(I2Cdev &i2cdev)
        : i2cdev(i2cdev),
          sensorSettings({
              sensorSetting{
                  xshutPinNum : 22,
                  intPinNum : 19,
                  address : 0x08,
              },
              sensorSetting{
                  xshutPinNum : 18,
                  intPinNum : 15,
                  address : 0x18,
              },
              sensorSetting{
                  xshutPinNum : 16,
                  intPinNum : 13,
                  address : 0x28,
              },
              sensorSetting{
                  xshutPinNum : 12,
                  intPinNum : 11,
                  address : 0x38,
              },
          })
    {
        int i = 0;
        for (auto &s : sensorSettings)
        {
            rangeSensors[i] = RangeSensor::instantiate(i2cdev, s.xshutPinNum, s.intPinNum);
            ++i;
        }
        timer::waitMs(500);
        std::cout << "start initializing range sernsor array" << std::endl;
        i = 0;
        for (auto &s : sensorSettings) {
            rangeSensors[i]->activate(s.address);
            ++i;
        }
        std::cout << "initializing range sernsor array ended" << std::endl;
    }

    std::vector<uint16_t> readRanges() override {
        std::vector<uint16_t> res;
        for(auto &s : rangeSensors) {
            auto range = s->readRangeAsMilliMeters();
            res.push_back(range);
        }
        return res;
    }
};

std::unique_ptr<RangeSensorArray> RangeSensorArray::instantiate(I2Cdev &i2cdev)
{
    return std::make_unique<RangeSensorArrayImpl>(i2cdev);
}