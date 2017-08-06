#include "gtest/gtest.h"
#include "MotionController.h"
#include "conf.h"
#include "MockMotionService.h"
#include "CommandBus.h"
#include <map>
#include <cstdint>

namespace
{
using ::testing::_;

class MotionControllerTest : public ::testing::Test
{
};

TEST_F(MotionControllerTest, onCommand)
{
    MockMotionService mockMotionService;
    EXPECT_CALL(mockMotionService, start(1)).Times(1);
    EXPECT_CALL(mockMotionService, stop()).Times(1);

    auto commandBus = CommandBus::instantiate();
    auto walkController = MotionController::instantiate(
        *commandBus,
        mockMotionService);

    std::map<int, uint16_t> payload{{0, 1}};
    commandBus->publish(Command{
        CommandType::MotionStart,
        payload});

    commandBus->publish(Command{
        CommandType::MotionStop,
        payload});
}
}