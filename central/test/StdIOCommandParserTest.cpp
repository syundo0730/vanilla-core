#include "gtest/gtest.h"
#include "StdIOCommandParser.h"
#include <iostream>
#include <sstream>

namespace
{

class StdIOCommandParserTest : public ::testing::Test
{
};

TEST_F(StdIOCommandParserTest, parse)
{
  auto stdIOParser = StdIOCommandParser::instantiate();
  std::stringstream ss;

  ss << "stop_motion" << std::endl;
  auto cmd = stdIOParser->parse(ss);
  EXPECT_EQ(CommandType::MotionStop, cmd.commandType);

  ss << "start_motion:1" << std::endl;
  cmd = stdIOParser->parse(ss);
  EXPECT_EQ(CommandType::MotionStart, cmd.commandType);
  EXPECT_EQ(1, cmd.payload[0]);

  ss << "show_joint_angles" << std::endl;
  cmd = stdIOParser->parse(ss);
  EXPECT_EQ(CommandType::ShowJointAngles, cmd.commandType);

  ss << "set_joint_angle:1,3142" << std::endl;
  cmd = stdIOParser->parse(ss);
  EXPECT_EQ(CommandType::SetJointAngle, cmd.commandType);
  EXPECT_EQ(1, cmd.payload[0]);
  EXPECT_EQ(3142, (int16_t)cmd.payload[1]);

  ss << "set_joint_angle:1,-3142" << std::endl;
  cmd = stdIOParser->parse(ss);
  EXPECT_EQ(CommandType::SetJointAngle, cmd.commandType);
  EXPECT_EQ(1, cmd.payload[0]);
  EXPECT_EQ(-3142, (int16_t)cmd.payload[1]);
}
}