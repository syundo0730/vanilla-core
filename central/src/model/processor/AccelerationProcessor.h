#ifndef ACCELERATION_PROCESSOR_H
#define ACCELERATION_PROCESSOR_H

#include <memory>
#include <array>

enum class FallState
{
  Stand,
  FallForward,
  FallBackword,
  FallLeft,
  FallRight,
  FallingForward,
  FallingBackword,
  FallingLeft,
  FallingRight,
};

class AccelerationProcessor
{
public:
  using t_acceleration = std::array<double, 3>;
  static std::unique_ptr<AccelerationProcessor> instantiate();
  virtual FallState process(const t_acceleration &) = 0;
};

#endif // ACCELERATION_PROCESSOR_H