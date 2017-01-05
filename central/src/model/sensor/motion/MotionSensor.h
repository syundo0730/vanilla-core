#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

#include <memory>
#include <Eigen/Dense>
class MPU6050;
class MadgwickQuaternionFilter;

class MotionSensor
{
public:
  static std::unique_ptr<MotionSensor> instantiate(
      MPU6050 &mpu, MadgwickQuaternionFilter &madgwickFilter);
  virtual Eigen::Quaterniond getQuaternion() = 0;
};

#endif // MOTION_SENSOR_H