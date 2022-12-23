#ifndef _RAY_H_
#define _RAY_H_
#include "defines.h"

class Ray {
public:
  /// origin point of ray
  Vec3f origin;
  /// normalized direction of the ray
  Vec3f direction;

  Ray(const Vec3f &o, const Vec3f &dir){
    origin=o;
    direction=dir.normalized();
  };

  Vec3f operator()(float t) const {
    return origin + t * direction;
  };
};
#endif