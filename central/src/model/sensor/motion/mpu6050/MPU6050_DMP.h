#ifndef MPU6050_DMP_H
#define MPU6050_DMP_H

#include "mraa.hpp"
#include "MPU6050_6Axis_MotionApps20.h"

class I2Cdev;
 
class MPU6050_DMP
{
private:
	I2Cdev &i2cdev;
	MPU6050 mpu;
	mraa::Gpio checkpin;
	bool dmpReady;  // set true if DMP init was successful
	uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
	volatile bool mpuInterrupt; // indicates whether MPU interrupt pin has gone high
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer

private:
	void onInterrupt();
	void read();

public:
	MPU6050_DMP(I2Cdev& i2cdev);
	void get();
};
 
#endif // MPU6050_DMP_H



