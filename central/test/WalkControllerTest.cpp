#include "gtest/gtest.h"
#include "WalkController.h"
#include "conf.h"
#include "MockWalkGenerator.h"
#include "MockLegControlService.h"
#include "CommandBus.h"
#include <map>
#include <cstdint>

namespace
{

class WalkControllerTest : public ::testing::Test
{
};

TEST_F(WalkControllerTest, onCommand)
{
    MockWalkGenerator mockWalkGenerator;
    MockLegControlService mockLegControlService;
    EXPECT_CALL(mockWalkGenerator, start()).Times(1);
    EXPECT_CALL(mockWalkGenerator, stop()).Times(1);

    auto commandBus = CommandBus::instantiate();
    auto walkController = WalkController::instantiate(
        *commandBus,
        mockWalkGenerator,
        mockLegControlService);

    std::map<int, uint16_t> payload{{1, 1}};
    commandBus->publish(Command{
        CommandType::WalkStart,
        payload});

    commandBus->publish(Command{
        CommandType::WalkStop,
        payload});
}
}