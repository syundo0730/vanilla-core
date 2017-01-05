#ifndef DIMENSIONAL_TYPES_H
#define DIMENSIONAL_TYPES_H

#include "Eigen/Dense"
#include <map>

// using Vector2 = Eigen::Vector2d;
// using Vector3 = Eigen::Vector3d;
using Vector2 = Eigen::Matrix<double,2,1,Eigen::DontAlign>;
using Vector3 = Eigen::Matrix<double,3,1,Eigen::DontAlign>;
using Vector2IntMap =  std::map<int, Vector2>;

#endif // DIMENSIONAL_TYPES_H