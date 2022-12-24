#ifndef _INTERACTION_H_
#define _INTERACTION_H_
#include "defines.h"
struct Interaction {
  enum Type {VOXEL, GEOMETRY, NONE};
  Vec3f pos{0, 0, 0};
  float value=0;
  float dist{RAY_DEFAULT_MAX};
  Type type{Type::NONE};
};
#endif 
