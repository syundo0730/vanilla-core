#include "gtest/gtest.h"
#include "MotionLoader.h"
#include <iostream>

namespace
{

class MotionLoaderTest : public ::testing::Test
{
};

TEST_F(MotionLoaderTest, initialize)
{
    auto l = MotionLoader::instantiate();
    auto motions = l->load("../motion.yaml");

    for (auto &m : motions)
    {
        std::cout << "ID:" << m.ID << std::endl;
        for (auto &p : m.Poses)
        {
            std::cout << "Interval: " << p.Interval << std::endl;
            for (auto &a : p.JointAnglesDeciDegree)
            {
                std::cout << a << ", ";
            }
            std::cout << std::endl;
        }
    }
}
}