#include "camera.h"
#include "ray.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

Image::Image(){
    resolution.x() = 800;
    resolution.y() = 600;
    RGBpixels.swap(std::vector<uint8_t>(resolution.x()*resolution.y()*3,0));
};

void Image::writefile(std::string filename){
    std::cout<<"Writing image...\n";
    stbi_write_png(filename.c_str(), resolution.x(), resolution.y(), 3, &RGBpixels[0], 0);
    std::cout<<"Image saved.\n";
};

void Image::setpixel(int x,int y,Vec3i RGBcolor){
    RGBpixels[(y*resolution.x()+x)*3] = RGBcolor.x();
    RGBpixels[(y*resolution.x()+x)*3+1] = RGBcolor.y();
    RGBpixels[(y*resolution.x()+x)*3+2] = RGBcolor.z();
};

void Camera::setFov(float new_fov){
    fov = new_fov;
};

void Camera::setPosition(Vec3f pos){
    position = pos;
};

void Camera::lookAt(Vec3f lookat, Vec3f refup){
    ref_up = refup;
    forward = (lookat - position).normalized();
    right = forward.cross(ref_up).normalized();
    up = right.cross(forward).normalized();
};

Camera::Camera(){
    position = {0,0,-1};
    fov = 45.0f;
    ref_up = {0,1,0};
    forward = {0,0,1};
    right = {1,0,0};
    up = {0,1,0};
    image = Image();
};

Camera::Camera(Vec3f pos,Vec3f lookat,float new_fov,Vec3f refup){
    position = pos;
    fov = new_fov;
    ref_up = refup;
    forward = (lookat - position).normalized();
    right = forward.cross(ref_up).normalized();
    up = right.cross(forward).normalized();
    image = Image();
};

void Camera::shotimage(Scene& scene){
    //todo: #1 implement a scene shot function.
    for(int i=0;i<image.resolution.y();i++){
        for(int j=0;j<image.resolution.x();j++) image.setpixel(j,i,Vec3i{255,255,0});
    }
    image.writefile("result.png");
};