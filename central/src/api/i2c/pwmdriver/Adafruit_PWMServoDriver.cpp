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

#include "Adafruit_PWMServoDriver.h"
#include "Timer.h"
#include <iostream>

class Adafruit_PWMServoDriverImpl : public Adafruit_PWMServoDriver
{
  private:
    int _i2caddr;
    I2Cdev &i2cdev;

  public:
    Adafruit_PWMServoDriverImpl(I2Cdev &_i2cdev)
        : _i2caddr(PCA9685_ADDRESS), i2cdev(_i2cdev) {}
    void i2c_probe(void) override
    {
        // printf("Searching for I2C devices...\n");

        int count = 0;
        for (int address = 4; address < 256; address += 2)
        {
            if (!i2cdev.writeByte(_i2caddr, address, 0))
            { // 0 returned is ok
                // printf(" - I2C device found at address 0x%02X\r\n", address);
                count++;
            }
        }
        // printf("%d devices found\r\n", count);
    }
    void begin(void) override
    {
        write8(PCA9685_MODE1, 0x0);
    }
    void setPWMFreq(float freq) override
    {
        //Serial.print("Attempting to set freq ");
        //Serial.println(freq);
        float prescaleval = 25000000;
        prescaleval /= 4096;
        prescaleval /= freq;
        //Serial.print("Estimated pre-scale: "); Serial.println(prescaleval);
        uint8_t prescale = floor(prescaleval + 0.5) - 1;
        //Serial.print("Final pre-scale: "); Serial.println(prescale);
        setPrescale(prescale);
    }
    void setPrescale(uint8_t prescale) override
    {
        uint8_t oldmode = read8(PCA9685_MODE1);
        uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
        write8(PCA9685_MODE1, newmode);            // go to sleep
        timer::waitMs(5);
        write8(PCA9685_PRESCALE, prescale); // set the prescaler
        write8(PCA9685_MODE1, oldmode);
        timer::waitMs(5);
        write8(PCA9685_MODE1, oldmode | 0xa1);
    }
    void setDuty(uint8_t num, uint16_t duty) override
    {
        float pulselength = 10000; // 10,000 us per second
        duty = 4094 * duty / pulselength;
        setPWM(num, 0, duty);
    }

  private:
    void setPWM(uint8_t channel, uint16_t on, uint16_t off)
    {
        uint8_t cmd[4];
        cmd[0] = on;
        cmd[1] = on >> 8;
        cmd[2] = off;
        cmd[3] = off >> 8;
        i2cdev.writeBytes(_i2caddr, LED0_ON_L + 4 * channel, 4, cmd);
    }
    uint8_t read8(char addr)
    {
        uint8_t rtn;
        i2cdev.readByte(_i2caddr, addr, &rtn);
        return rtn;
    }
    void write8(char addr, char d)
    {
        i2cdev.writeByte(_i2caddr, addr, d);
    }
};

std::unique_ptr<Adafruit_PWMServoDriver> Adafruit_PWMServoDriver::instantiate(I2Cdev &i2cdev) {
    return std::make_unique<Adafruit_PWMServoDriverImpl>(i2cdev);
}