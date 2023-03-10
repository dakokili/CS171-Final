#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <Eigen/Core>
#include <Eigen/Dense>

#include <iostream>
#include <vector>
#include <cassert>
#include <string>

using Mat3f = Eigen::Matrix3f;
using Mat3d = Eigen::Matrix3d;
using Mat4d = Eigen::Matrix4d;
using Vec3f = Eigen::Vector3f;
using Vec3d = Eigen::Vector3d;
using Vec3i = Eigen::Vector3i;
using Vec2i = Eigen::Vector2i;
using Vec2f = Eigen::Vector2f;

constexpr bool velocity=true;        //false for Q-criterion, true for velocity
constexpr bool linearorbspline=false; //flase for b-spline, true for linear
constexpr float transparency=0.1f;

constexpr int SPP=10;
constexpr float RAY_DEFAULT_MIN = 1e-5;
constexpr float RAY_DEFAULT_MAX = 30;
constexpr float PI = 3.141592653579f;
constexpr float INV_PI = 0.31830988618379067154;

#define LOG_INFO(msg)                             \
  std::cerr << "[INFO] "                          \
            << "function(" << __FUNCTION__ << ")" \
            << " line " << __LINE__ << ": " << (msg) << std::endl

#define LOG_ERR(msg)                                                 \
  do {                                                               \
    std::cerr << "[ERROR] "                                          \
              << "function(" << __FUNCTION__ << ")"                  \
              << " line " << __LINE__ << ": " << (msg) << std::endl; \
    std::exit(1);                                                    \
  } while (0)

#define UNIMPLEMENTED                            \
  do {                                           \
    std::cerr << "not implemented" << std::endl; \
    assert(false);                               \
  } while (0)

#endif