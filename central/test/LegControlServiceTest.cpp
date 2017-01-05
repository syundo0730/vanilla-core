#include "gtest/gtest.h"
#include "LegControlService.h"
#include "LegKinematics.h"
#include "conf.h"
#include "dimensional_types.h"
#include "MockJointRepository.h"

namespace
{

using ::testing::_;

class LegControlServiceTest : public ::testing::Test
{
};

TEST_F(LegControlServiceTest, setEndEffectorPosition)
{
  MockJointRepository mockJointRepository;
  EXPECT_CALL(mockJointRepository, setTargetJointAngle(_, _)).Times(12);

  auto conf = Conf::defaultConf();
  auto leftLegKinematics = LegKinematics::instantiate(conf.LegKinematics.LegRootLeft, conf);
  auto rightLegKinematics = LegKinematics::instantiate(conf.LegKinematics.LegRootRight, conf);
  auto legControlService = LegControlService::instantiate(
      mockJointRepository,
      *leftLegKinematics,
      *rightLegKinematics,
      conf);

  auto leftTarget = Vector3(0, 0.03, -0.1);
  auto rightTarget = Vector3(0, -0.03, -0.1);
  legControlService->setEndEffectorPosition(leftTarget, rightTarget);
}
}