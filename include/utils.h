#ifndef _UTILS_H_
#define _UTILS_H_
#include "defines.h"
#include <random>
/**
 * try and search target file in parent directories with max depth limit
 */
std::string getPath(const std::string &target, int depth = 5);
class Sampler {
public:
  Sampler() = default;
  float get1D() { return dis(engine); }
  void setSeed(int i) { engine.seed(i); }
private:
  std::default_random_engine engine;
  std::uniform_real_distribution<float> dis;
};
Vec3i converttoRGB(Vec3f color);
#endif