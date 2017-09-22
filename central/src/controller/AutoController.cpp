#include "AutoController.h"
#include "ActiveRangeSensorArray.h"
#include "MotionSensor.h"
#include "AccelerationProcessor.h"
#include "RangeDataProcessor.h"
#include "CommandBus.h"
#include <functional>
#include <iostream>
#include "InvalidRangeException.h"

class AutoControllerImpl : public AutoController
{
  private:
    ActiveRangeSensorArray &activeRangeSensorArray;
    MotionSensor &motionSensor;
    AccelerationProcessor &accelerationProcessor;
    RangeDataProcessor &rangeDataProcessor;
    CommandBus &commandBus;

  private:
    using t_ranges = std::vector<uint16_t>;
    bool initialized;

  public:
    AutoControllerImpl(
        ActiveRangeSensorArray &activeRangeSensorArray,
        MotionSensor &motionSensor,
        AccelerationProcessor &accelerationProcessor,
        RangeDataProcessor &rangeDataProcessor,
        CommandBus &commandBus)
        : activeRangeSensorArray(activeRangeSensorArray),
          motionSensor(motionSensor),
          accelerationProcessor(accelerationProcessor),
          rangeDataProcessor(rangeDataProcessor),
          commandBus(commandBus),
          initialized(false)
    {
        activeRangeSensorArray.setListener(
            std::bind(&AutoControllerImpl::onRangeSensorRead, this, std::placeholders::_1));
        activeRangeSensorArray.moveToInitialPosition();
    }
    void update() override
    {
        activeRangeSensorArray.update();
        motionSensor.update();
    }

  private:
    void onRangeSensorRead(const t_ranges &ranges)
    {
        auto inertialInfo = motionSensor.getInertialInfo();
        using t_acc = AccelerationProcessor::t_acceleration;
        t_acc acc = {inertialInfo[0], inertialInfo[1], inertialInfo[2]};
        auto fallState = accelerationProcessor.process(acc);
        try
        {
            auto rangeState = rangeDataProcessor.process(ranges);
            auto str = stateAsString(fallState, rangeState.Range);
            std::cout << str << rangeState.TargetIndex << std::endl;
        }
        catch (const InvalidRangeException &e)
        {
            std::cout << e.getIndex() << " : " << e.what() << std::endl;
            return;
        }
    }
    std::string stateAsString(FallState fs, Range rs)
    {
        std::string str = "";
        switch(fs) {
            case FallState::Stand:
                str += "stand, ";
                break;
            case FallState::FallForward:
                str += "fallForward, ";
                break;
            case FallState::FallBackword:
                str += "fallBackword, ";
                break;
            case FallState::FallLeft:
                str += "fallLeft, ";
                break;
            case FallState::FallRight:
                str += "fallRight, ";
                break;
            case FallState::FallingForward:
                str += "fallingForward, ";
                break;
            case FallState::FallingBackword:
                str += "fallingBackword, ";
                break;
            case FallState::FallingLeft:
                str += "fallingLeft, ";
                break;
            case FallState::FallingRight:
                str += "fallingRight, ";
                break;
        }
        switch(rs) {
            case Range::Near:
                str += "near, ";
                break;
            case Range::Detectable:
                str += "detectable, ";
                break;
            case Range::Far:
                str += "far, ";
                break;
        }
        return str;
    }
};

std::unique_ptr<AutoController> AutoController::instantiate(
    ActiveRangeSensorArray &activeRangeSensorArray,
    MotionSensor &motionSensor,
    AccelerationProcessor &accelerationProcessor,
    RangeDataProcessor &rangeDataProcessor,
    CommandBus &commandBus)
{
    return std::make_unique<AutoControllerImpl>(
        activeRangeSensorArray,
        motionSensor,
        accelerationProcessor,
        rangeDataProcessor,
        commandBus);
}