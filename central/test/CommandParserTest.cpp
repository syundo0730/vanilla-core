#include "gtest/gtest.h"
#include "CommandParser.h"
#include "utility.h"
#include <vector>

namespace
{

class CommandParserTest : public ::testing::Test
{
};

uint8_t* makeCommand(uint8_t command, std::vector<uint16_t> body) {
    uint8_t bodyLen = body.size();
    uint8_t* data = new uint8_t[bodyLen * 2 + 5];
    data[0] = 0xFA;
    data[1] = 0xAF;
    data[2] = command;
    data[3] = bodyLen;
    int i = 4;
    for (auto &b : body) {
        auto bvec = utility::devideToByte<
            std::vector<uint8_t>,
            uint16_t>(b);
        data[i] = bvec[0];
        data[i + 1] = bvec[1];
        i += 2;
    }
    int last = 4 + bodyLen * 2;
    uint8_t sum = data[2];
    for (int i = 3; i < last; ++i) {
        sum = sum ^ data[i];
    }
    data[last] = sum;
    return data;
}

TEST_F(CommandParserTest, parse)
{
  auto cp = CommandParser::instantiate();

  std::vector<uint16_t> body0;
  body0.push_back(1000);
  auto data0 = makeCommand(0x01, body0);
  auto cmd0 = cp->parse(data0, 7);
  EXPECT_EQ(CommandType::WalkStop, cmd0.commandType);
  EXPECT_EQ(1000, cmd0.payload[0]);

  std::vector<uint16_t> body1;
  body1.push_back(1000);
  auto data1 = makeCommand(0x02, body1);
  auto cmd1 = cp->parse(data1, 7);
  EXPECT_EQ(CommandType::WalkStart, cmd1.commandType);
  EXPECT_EQ(1000, cmd1.payload[0]);

  std::vector<uint16_t> body2;
  body2.push_back(1111);
  body2.push_back(2222);
  auto data2 = makeCommand(0x01, body2);
  auto cmd2 = cp->parse(data2, 9);
  EXPECT_EQ(CommandType::WalkStop, cmd2.commandType);
  EXPECT_EQ(1111, cmd2.payload[0]);
  EXPECT_EQ(2222, cmd2.payload[1]);
}
}