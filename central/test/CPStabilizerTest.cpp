#include "gtest/gtest.h"
#include "CPStabilizer.h"
#include "conf.h"
#include "dimensional_types.h"

namespace
{

class CPStabilizerTest : public ::testing::Test
{
};

TEST_F(CPStabilizerTest, stabilize)
{
  auto conf = Conf::defaultConf();
  auto s = CPStabilizer::instantiate(conf);
  auto val = s->stabilize(Vector2(0, 0), Vector2(0, 0));
  EXPECT_EQ(0, val);
}
}