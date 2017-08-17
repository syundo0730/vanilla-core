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

  ss << "a" << std::endl;
  auto cmd0 = stdIOParser->parse(ss);
  EXPECT_EQ(CommandType::MotionStart, cmd0.commandType);
  EXPECT_EQ(0, cmd0.payload[0]);

  ss << "b" << std::endl;
  auto cmd1 = stdIOParser->parse(ss);
  EXPECT_EQ(CommandType::MotionStart, cmd1.commandType);
  EXPECT_EQ(0, cmd0.payload[0]);

  ss << "c" << std::endl;
  auto cmd2 = stdIOParser->parse(ss);
  EXPECT_EQ(CommandType::MotionStop, cmd2.commandType);
}
}