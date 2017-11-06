#include "gtest/gtest.h"
#include "Interpolator.h"
#include "dimensional_types.h"

namespace
{

class InterpolatorTest : public ::testing::Test
{
};

TEST_F(InterpolatorTest, getPosition)
{
  auto interp = Interpolator::instantiate();

  auto Tdbl = 0.2;
  interp->initialize(
      Vector3(0, -0.00303947, 0.8),
      Vector3(0, 0.297942, 0),
      Vector3(0, -0.000247953, 0),
      Tdbl);

  auto dt = 0.01;
  for (int i = 0; i < 20; ++i) {
    auto pos = interp->getPosition(i * dt);
    std::cout
        << pos[0] << ", "
        << pos[1] << ", "
        << pos[2] << std::endl;
  }
}
}