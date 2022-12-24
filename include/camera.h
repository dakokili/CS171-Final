#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "defines.h"
#include "scene.h"

class Image{
public:
  Image();
  void writefile(std::string filename);
  void setpixel(int x,int y,Vec3i RGBcolor);
  Vec2i resolution;
  std::vector<uint8_t> RGBpixels;
};

class Camera {
 public:
  Camera();
  Camera(Vec3f pos,Vec3f lookat,float new_fov=45.0f,Vec3f refup={0,1,0});
  void lookAt(Vec3f lookat, Vec3f refup = {0, 1, 0});
  void setPosition(Vec3f pos);
  void setFov(float new_fov);
  void shotimage(Scene& scene);
  openvdb::math::Ray<float> generateray(int x,int y);
  Vec3i transferfunction(std::vector<Interaction>& interactions);
 private:
  Vec3f position;
  Vec3f forward;
  Vec3f up;
  Vec3f right;
  Vec3f ref_up;
  float fov;
  Image image;
};

#endif // CAMERA_H_
