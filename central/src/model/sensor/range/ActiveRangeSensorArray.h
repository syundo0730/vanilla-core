#ifndef ACTIVE_RANGE_SENSOR_ARRAY_H
#define ACTIVE_RANGE_SENSOR_ARRAY_H

#include <memory>
#include <cstdint>
#include <vector>
#include <functional>

class RangeSensorArray;
class JointRepository;

class ActiveRangeSensorArray
{
private:
  using t_ranges = std::vector<uint16_t>;

public:
  using Listener = std::function<void(const t_ranges &)>;

  static std::unique_ptr<ActiveRangeSensorArray> instantiate(
      RangeSensorArray &rangeSensorArray,
      JointRepository &jointRepository,
      int jointID);
  virtual void moveToInitialPosition() = 0;
  virtual void update() = 0;
  virtual void setListener(Listener) = 0;
};

#endif // ACTIVE_RANGE_SENSOR_ARRAY_H