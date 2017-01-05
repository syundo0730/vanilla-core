#ifndef MOCK_Adafruit_PWM_SERBO_DRIVER_H
#define MOCK_Adafruit_PWM_SERBO_DRIVER_H

#include "gmock/gmock.h"
#include "Adafruit_PWMServoDriver.h"
#include <cstdint>

class MockAdafruit_PWMServoDriver : public Adafruit_PWMServoDriver
{
  public:
    MOCK_METHOD0(i2c_probe, void(void));
    MOCK_METHOD0(begin, void(void));
    MOCK_METHOD1(setPWMFreq, void(float));
    MOCK_METHOD1(setPrescale, void(uint8_t));
    MOCK_METHOD2(setDuty, void(uint8_t, uint16_t));
};
#endif