#include "AutoController.h"
#include "ActiveRangeSensorArray.h"
#include "CommandBus.h"
#include <functional>

class AutoControllerImpl : public AutoController
{
  private:
    using t_ranges = std::vector<uint16_t>;
    ActiveRangeSensorArray &activeRangeSensorArray;
    CommandBus &commandBus;

  public:
    AutoControllerImpl(
        ActiveRangeSensorArray &activeRangeSensorArray,
        CommandBus &commandBus)
        : activeRangeSensorArray(activeRangeSensorArray),
          commandBus(commandBus)
    {
        activeRangeSensorArray.moveToInitialPosition();
        std::function<void(const t_ranges&)> onRead = std::bind(
            &AutoControllerImpl::onRangeSensorRead,
            this,
            std::placeholders::_1);
        activeRangeSensorArray.setListener(&onRead);
    }
    void update() override
    {
        activeRangeSensorArray.update();
    }

  private:
    void onRangeSensorRead(const t_ranges &data)
    {
        auto command = processSensorData(data);
        commandBus.publish(command);
    }
    Command processSensorData(const std::vector<uint16_t> &data)
    {
        return createCommand(CommandType::MotionStart, 3);
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