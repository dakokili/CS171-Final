#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <openvdb/openvdb.h>

#include <iostream>
#include <vector>
#include <cassert>

//template <typename T, int size>
//using Vec = Eigen::Matrix<T, size, 1>;
using File = openvdb::io::File;
using Mat3f = Eigen::Matrix3f;
using Mat3d = Eigen::Matrix3d;
using Mat4d = Eigen::Matrix4d;
using Vec3s = Eigen::Vector3f;
using Vec3d = Eigen::Vector3d;
using Vec3i = Eigen::Vector3i;


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