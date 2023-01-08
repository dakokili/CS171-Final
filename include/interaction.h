#ifndef _INTERACTION_H_
#define _INTERACTION_H_
#include "defines.h"
struct Interaction {
  enum Type {VOXEL, GEOMETRY, NONE};
  Vec3f pos{0, 0, 0};
  Vec3f normal{0,0,0};
  float value=0;
  float scale=0;
  float dist{RAY_DEFAULT_MAX};
  Type type{Type::NONE};
  //bool inside(Vec3f bottom,Vec3f top){
  //  return (pos[0]>=bottom[0])&&(pos[1]>=bottom[1])&&(pos[2]>=bottom[2])
  //  &&(pos[0]<=top[0])&&(pos[1]<=top[1])&&(pos[2]<=top[2]);
  //};
};
#endif 
