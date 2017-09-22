#include "ActiveRangeSensorArray.h"
#include "RangeSensorArray.h"
#include "JointRepository.h"
#include "JointAngle.h"
#include "Timer.h"
#include <iostream>

class ActiveRangeSensorArrayImpl : public ActiveRangeSensorArray
{
  private:
    RangeSensorArray &rangeSensorArray;
    JointRepository &jointRepository;
    int jointID;

    bool moveClockwise, initialized;
    int currentStep;
    int16_t currentAngle;
    std::vector<uint16_t> ranges;
    Listener listener;

    static const int16_t RANGE_UPPER = 450;
    static const int16_t RANGE_LOWER = -450;
    static const uint16_t RANGE_DIFF = 100;
    static const uint8_t SENSOR_NUM = 4;
    static const uint16_t SCAN_NUM = (RANGE_UPPER - RANGE_LOWER) / RANGE_DIFF;
    static const uint16_t RANGE_NUM = SENSOR_NUM * SCAN_NUM;

  public:
    ActiveRangeSensorArrayImpl(
        RangeSensorArray &rangeSensorArray,
        JointRepository &jointRepository,
        int jointID)
        : rangeSensorArray(rangeSensorArray),
          jointRepository(jointRepository),
          jointID(jointID),
          moveClockwise(true),
          initialized(false),
          currentStep(0),
          currentAngle(0),
          ranges(RANGE_NUM, 0),
          listener(nullptr)
    {
    }

    void moveToInitialPosition() override
    {
        auto targetAngle = -RANGE_UPPER;
        setTargetJointAngle(targetAngle);
        currentStep = 0;
        currentAngle = targetAngle;
        initialized = true;
    }

    void update() override
    {
        if (!initialized)
        {
            return;
        }
        auto ranges = rangeSensorArray.readRanges();
        assignRanges(ranges);
        moveJoint();
        ++currentStep;
        if (currentStep == SCAN_NUM)
        {
            onScanEnded();
        }
    }

    void setListener(Listener l) override
    {
        listener = l;
    }

  private:
    void setTargetJointAngle(int16_t angle)
    {
        jointRepository.setTargetJointAngle(jointID, JointAngle(angle));
    }

    void onScanEnded()
    {
        listener(ranges);
        moveClockwise = !moveClockwise;
        currentStep = 0;
    }

    void assignRanges(const std::vector<uint16_t> &src)
    {
        int i = 0;
        for (auto &r : src)
        {
            int index = moveClockwise ? i * SCAN_NUM + currentStep : (i + 1) * SCAN_NUM - currentStep;
            if (index == RANGE_NUM)
            {
                index = 0;
            }
            ranges[index] = r;
            ++i;
        }
    }

    void moveJoint()
    {
        int diff = moveClockwise ? RANGE_DIFF : -RANGE_DIFF;
        currentAngle += diff;
        setTargetJointAngle(currentAngle);
    }
};

std::unique_ptr<ActiveRangeSensorArray> ActiveRangeSensorArray::instantiate(
    RangeSensorArray &rangeSensorArray,
    JointRepository &jointRepository,
    int jointID)
{
    return std::make_unique<ActiveRangeSensorArrayImpl>(
        rangeSensorArray,
        jointRepository,
        jointID);
}