#include "utils.h"

#ifdef USE_FILESYSTEM
#include <filesystem>

std::string getPath(const std::string &target, int depth) {
  std::string path = target;
  namespace fs = std::filesystem;
  for (int i = 0; i < depth; ++i) {
    if (fs::exists(path)) {
      return path;
    }
    path = "../" + path;
  }
  LOG_ERR("failed to get file: " + target);
  return target;
}
#else
#include <cstdio>

std::string getPath(const std::string &target, int depth) {
  std::string path = target;
  for (int i = 0; i < depth; ++i) {
    FILE *file = fopen(path.c_str(), "r");
    if (file) {
      fclose(file);
      return path;
    }
    path = "../" + path;
  }
  LOG_ERR("failed to get file: " + target);
  return target;
}

Vec3i converttoRGB(Vec3f color){
  if(color.x()>1.0f) color.x()=1.0f;
  if(color.y()>1.0f) color.y()=1.0f;
  if(color.z()>1.0f) color.z()=1.0f;
  Vec3i ans;
  ans.x()=powf(color.x(),1.f/2.2f);
  ans.y()=powf(color.y(),1.f/2.2f);
  ans.z()=powf(color.z(),1.f/2.2f);
  return ans;
};
#endif