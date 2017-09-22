#ifndef AUTO_CONTROLLER_H_
#define AUTO_CONTROLLER_H_

#include <memory>

class CommandBus;
class ActiveRangeSensorArray;
class MotionSensor;
class AccelerationProcessor;
class RangeDataProcessor;

class AutoController
{
  public:
    static std::unique_ptr<AutoController> instantiate(
        ActiveRangeSensorArray &,
        MotionSensor &,
        AccelerationProcessor &,
        RangeDataProcessor &,
        CommandBus &);
    virtual void update() = 0;
};

#endif
