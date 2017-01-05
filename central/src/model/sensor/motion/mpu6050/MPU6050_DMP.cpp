#include "MPU6050_DMP.h"
#include "I2Cdev.h"
#include <iostream>
#include <cmath>
#include <functional>

MPU6050_DMP::MPU6050_DMP(I2Cdev& _i2cdev):
i2cdev(_i2cdev),
mpu(i2cdev),
checkpin(11),
// pin number: refer here
// https://iotdk.intel.com/docs/master/mraa/rasppi.html
dmpReady(false),
mpuInterrupt(false)
{
    std::cout << "Initializing I2C devices..." << std::endl;
    mpu.initialize();

    // verify connection
    std::cout << "Testing device connections..." << std::endl;
    std::cout << (mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed") << std::endl;

    // load and configure the DMP
    std::cout << "Initializing DMP..." << std::endl;
    uint8_t devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    std::cout << "Setting Gyro Offsets..." << std::endl;
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        std::cout << "Enabling DMP..." << std::endl;
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        std::cout << "Enabling interrupt detection (Arduino external interrupt 0)..." << std::endl;

		// set interrupt
		checkpin.dir(mraa::DIR_IN);
		std::function<void(void)> func = std::bind(&MPU6050_DMP::onInterrupt, this);
		checkpin.isr(
			mraa::EDGE_RISING,
			*func.target<void(*)(void*)>(),
			nullptr);

        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        std::cout << "DMP ready! Waiting for first interrupt..." << std::endl;
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        std::cout << "DMP Initialization failed (code ";
        std::cout << devStatus;
        std::cout << ")" << std::endl;
    }
}

void MPU6050_DMP::get() {
    if (!dmpReady) return;

    while (!mpuInterrupt && fifoCount < packetSize);

    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        std::cout << "FIFO overflow!" << std::endl;

        // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

		read();
	}
}

void MPU6050_DMP::read() {
    Quaternion q;	// [w, x, y, z]         quaternion container
    // VectorInt16 aa;      // [x, y, z]            accel sensor measurements
    // VectorInt16 aaReal;  // [x, y, z]            gravity-free accel sensor measurements
    // VectorInt16 aaWorld; // [x, y, z]            world-frame accel sensor measurements
    // VectorFloat gravity; // [x, y, z]            gravity vector
    float euler[3];      // [psi, theta, phi]    Euler angle container
    // float ypr[3];	// [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetEuler(euler, &q);
    std::cout << "euler\t";
    std::cout << (euler[0] * 180 / M_PI);
    std::cout << "\t";
    std::cout << (euler[1] * 180 / M_PI);
    std::cout << "\t";
    std::cout << (euler[2] * 180 / M_PI) << std::endl;
}

void MPU6050_DMP::onInterrupt() {
	mpuInterrupt = true;
}