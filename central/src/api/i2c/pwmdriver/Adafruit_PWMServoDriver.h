/*************************************************** 
  This is a library for our Adafruit 16-channel PWM & Servo driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/*****************************
  This program was ported from https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library.
  I also added some functions.
  Shundo Kishi
 *****************************/

#ifndef _ADAFRUIT_PWM_SERVO_DRIVER_H
#define _ADAFRUIT_PWM_SERVO_DRIVER_H

#include <memory>
#include "I2Cdev.h"
#include <cmath>

#define PCA9685_ADDRESS 0x40
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

class Adafruit_PWMServoDriver
{
public:
  static std::unique_ptr<Adafruit_PWMServoDriver> instantiate(I2Cdev &i2cdev);
  virtual void i2c_probe(void) = 0;
  virtual void begin(void) = 0;
  virtual void setPWMFreq(float freq) = 0;
  virtual void setPrescale(uint8_t prescale) = 0;
  virtual void setDuty(uint8_t channel, uint16_t duty) = 0;
};

#endif