#include "AutoController.h"
#include "ActiveRangeSensorArray.h"
#include "CommandBus.h"
#include <functional>
#include <iostream>

class AutoControllerImpl : public AutoController
{
  private:
    using t_ranges = std::vector<uint16_t>;
    ActiveRangeSensorArray &activeRangeSensorArray;
    CommandBus &commandBus;
    bool initialized;

  public:
    AutoControllerImpl(
        ActiveRangeSensorArray &activeRangeSensorArray,
        CommandBus &commandBus)
        : activeRangeSensorArray(activeRangeSensorArray),
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
    }

  private:
    void onRangeSensorRead(const t_ranges &data)
    {
        // auto command = processSensorData(data);
        // commandBus.publish(command);
        for (auto v : data) {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
    }
    Command processSensorData(const std::vector<uint16_t> &data)
    {
        return createCommand(CommandType::MotionStart, 0);
    }
};

std::unique_ptr<AutoController> AutoController::instantiate(
    ActiveRangeSensorArray &activeRangeSensorArray,
    CommandBus &commandBus)
{
    return std::make_unique<AutoControllerImpl>(
        activeRangeSensorArray,
        commandBus);
}