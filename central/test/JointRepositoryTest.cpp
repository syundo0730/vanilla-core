#include "gtest/gtest.h"
#include "JointRepository.h"
#include "conf.h"
#include "MockRSApi.h"
#include "MockAdafruit_PWMServoDriver.h"
#include "JointAngle.h"

namespace
{
using ::testing::_;

class JointRepositoryTest : public ::testing::Test
{
};

TEST_F(JointRepositoryTest, applyTargetAngle)
{
    MockRSApi mockRSApi;
    MockAdafruit_PWMServoDriver mockPWMDriver;
    EXPECT_CALL(mockRSApi, on()).Times(1);
    EXPECT_CALL(mockRSApi, sendMultiPosition(_, _)).Times(1);
    EXPECT_CALL(mockPWMDriver, begin()).Times(1);
    EXPECT_CALL(mockPWMDriver, setPrescale(_)).Times(1);
    EXPECT_CALL(mockPWMDriver, setDuty(_, _)).Times(8);

    auto conf = Conf::defaultConf();
    auto jointRepository = JointRepository::instantiate(
        mockRSApi,
        mockPWMDriver,
        conf);
    jointRepository->setTargetJointAngle(0, JointAngle(0));
    jointRepository->setTargetJointAngle(4, JointAngle(0));
    jointRepository->setTargetJointAngle(8, JointAngle(0));
    jointRepository->applyTargetAngle();
}
}