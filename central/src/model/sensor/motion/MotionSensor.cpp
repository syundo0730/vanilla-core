#include "MotionSensor.h"
#include "MPU6050_DMP.h"
#include "MadgwickQuaternionFilter.h"
#include <iostream>
#include <cmath>
#include <functional>

class MotionSensorImpl : public MotionSensor
{
  private:
	MPU6050 &mpu;
	MadgwickQuaternionFilter &madgwickFilter;
	const double aRes = 2.0 / 32768.0;
	const double gRes = 250.0 / 32768.0 * M_PI / 180.0;

  public:
	MotionSensorImpl(MPU6050 &mpu, MadgwickQuaternionFilter &madgwickFilter)
		: mpu(mpu), madgwickFilter(madgwickFilter)
	{
		// initialize device
		std::cout << "Initializing I2C devices..." << std::endl;
		mpu.initialize();
		mpu.setXGyroOffset(220);
		mpu.setYGyroOffset(76);
		mpu.setZGyroOffset(-85);
		mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

		// verify connection
		std::cout << "Testing device connections..." << std::endl;
		std::cout << (mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed") << std::endl;
	}
	Eigen::Quaterniond getQuaternion() override
	{
		int16_t ax_raw, ay_raw, az_raw;
		int16_t gx_raw, gy_raw, gz_raw;
		mpu.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
		float ax = ax_raw * aRes;
		float ay = ay_raw * aRes;
		float az = az_raw * aRes;
		float gx = gx_raw * gRes;
		float gy = gy_raw * gRes;
		float gz = gz_raw * gRes;
		madgwickFilter.update(ax, ay, az, gx, gy, gz);
		return madgwickFilter.getAsQuaternion();
	}
};

std::unique_ptr<MotionSensor> MotionSensor::instantiate(
	MPU6050 &mpu, MadgwickQuaternionFilter &madgwickFilter)
{
    return std::make_unique<MotionSensorImpl>(mpu, madgwickFilter);
}