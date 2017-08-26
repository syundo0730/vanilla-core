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
                  xshutPinNum : 11,
                  intPinNum : 18,
                  address : 0x20,
              },
              sensorSetting{
                  xshutPinNum : 12,
                  intPinNum : 18,
                  address : 0x30,
              },
              sensorSetting{
                  xshutPinNum : 15,
                  intPinNum : 18,
                  address : 0x40,
              },
              sensorSetting{
                  xshutPinNum : 16,
                  intPinNum : 18,
                  address : 0x50,
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
        std::cout << "init done" << std::endl;
        i = 0;
        for (auto &s : sensorSettings) {
            rangeSensors[i]->activate(s.address);
            ++i;
        }
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