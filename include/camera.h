#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "defines.h"
#include "scene.h"

class Image{
public:
  Image();
  ~Image();
  void writefile();
  Vec2i resolution;
  Vec3i* RGBpixels;
};

class Camera {
 public:
  Camera(Vec3f pos,Vec3f lookat,float new_fov=45.0f,Vec3f refup={0,1,0});
  void lookAt(Vec3f lookat, Vec3f refup = {0, 1, 0});
  void setPosition(Vec3f pos);
  void setFov(float new_fov);

 private:
  Vec3f position={0,0,-1};
  Vec3f forward={0,0,1};
  Vec3f up={0,1,0};
  Vec3f right={1,0,0};
  Vec3f ref_up={0,1,0};
  float fov=45.0f;
  Image image;
  void shotimage(Scene& scene);
};

#endif // CAMERA_H_
