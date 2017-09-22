#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

#include <memory>
#include <Eigen/Dense>
class MPU6050;
class MadgwickQuaternionFilter;

class MotionSensor
{
public:
  using t_inertial_info = std::array<double, 6>;
  static std::unique_ptr<MotionSensor> instantiate(
      MPU6050 &mpu, MadgwickQuaternionFilter &madgwickFilter);
  virtual void update() = 0;
  virtual t_inertial_info getInertialInfo() = 0;
  virtual Eigen::Quaterniond getQuaternion() = 0;
};

#endif // MOTION_SENSOR_H