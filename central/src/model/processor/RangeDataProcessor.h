#ifndef RANGE_SENSOR_PROCESSOR_H
#define RANGE_SENSOR_PROCESSOR_H

#include <memory>
#include <vector>
#include <array>
#include <cstdint>

enum class Range
{
  Near,
  Detectable,
  Far,
};

using t_range = Range;
struct RangeState
{
  size_t TargetIndex;
  t_range Range;
};

class RangeDataProcessor
{
public:
  using t_ranges = std::vector<uint16_t>;

  static std::unique_ptr<RangeDataProcessor> instantiate();
  virtual RangeState process(const t_ranges &) = 0;
};

#endif // RANGE_SENSOR_PROCESSOR_H