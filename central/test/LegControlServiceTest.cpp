#include "gtest/gtest.h"
#include "LegControlService.h"
#include "LegKinematics.h"
#include "conf.h"
#include "dimensional_types.h"
#include "JointRepositorySpy.h"
#include <iostream>

namespace
{

class LegControlServiceTest : public ::testing::Test
{
};

TEST_F(LegControlServiceTest, setEndEffectorPosition)
{
  auto jointRepositorySpy = JointRepositorySpy::instantiate();

  auto conf = Conf::defaultConf();
  auto leftLegKinematics = LegKinematics::instantiate(conf.LegKinematics.LegRootLeft, conf);
  auto rightLegKinematics = LegKinematics::instantiate(conf.LegKinematics.LegRootRight, conf);
  auto legControlService = LegControlService::instantiate(
      *jointRepositorySpy,
      *leftLegKinematics,
      *rightLegKinematics,
      conf);

  auto leftTarget = Vector3(0, 0.06, -0.1);
  auto rightTarget = Vector3(0, 0, -0.1);
  legControlService->setEndEffectorPosition(leftTarget, rightTarget);

  auto angles = jointRepositorySpy->getJointAnglesAsDeciDegree();

  int expected[12] = {0, -309, -798, -1597, 798, -309, 0, -309, 798, 1597, -798, -309};
  for (auto i = 0; i < 12; ++i) {
    EXPECT_EQ(expected[i], angles[i]);
  }

  std::array<std::string, 6> leftJointNames{{"ll0", "ll1", "ll2", "ll3", "ll4", "ll5"}};
  std::array<std::string, 6> rightJointNames{{"lr0", "lr1", "lr2", "lr3", "lr4", "lr5"}};

  for (int i = 0; i < 6; ++i) {
    std::cout << leftJointNames[i] << ", ";
  }
  for (int i = 0; i < 6; ++i) {
    std::cout << rightJointNames[i] << ", ";
  }
  std::cout << std::endl;

  auto jointIDMap = conf.Joint.IDMap;
  for (int i = 0; i < 6; ++i) {
    auto id = jointIDMap[leftJointNames[i]];
    std::cout << angles[id] << ", ";
  }
  for (int i = 0; i < 6; ++i) {
    auto id = jointIDMap[rightJointNames[i]];
    std::cout << angles[id] << ", ";
  }
  std::cout << std::endl;
}
}