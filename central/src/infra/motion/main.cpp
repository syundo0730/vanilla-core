#include <iostream>
#include <fstream>
#include "WalkGenerator.h"

int main()
{
  WalkGenerator<double> walkgen(5);
  walkgen.setWalkParam(1, Eigen::Vector2d(0, 0.2));
  walkgen.setWalkParam(2, Eigen::Vector2d(0.2, 0.2));
  walkgen.setWalkParam(3, Eigen::Vector2d(0.2, 0.2));
  walkgen.setWalkParam(4, Eigen::Vector2d(0.2, 0.2));
  walkgen.setWalkParam(5, Eigen::Vector2d(0, 0.2));
  walkgen.setInitialParam(0.8, 0.8, Eigen::Vector2d(0, 0));
  walkgen.execOptimization(10, 1);
  walkgen.writeCogToStream(std::ofstream("result.csv"));
}