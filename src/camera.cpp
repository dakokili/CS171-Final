#include "camera.h"
#include "ray.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

Image::Image(){
    resolution[0]=800;
    resolution[1]=600;
    RGBpixels=(Vec3i*)calloc(1,resolution.x()*resolution.y()*sizeof(Vec3i));
};

Image::~Image(){
    free(RGBpixels);
};

void Image::writefile(){
    std::cout<<"Writing image...\n";
    stbi_write_png("result.png", resolution.x(), resolution.y(), 3, RGBpixels, 0);
    std::cout<<"Image saved.\n";
};

void Camera::setFov(float new_fov){
    fov=new_fov;
};

void Camera::setPosition(Vec3f pos){
    position=pos;
};

void Camera::lookAt(Vec3f lookat, Vec3f refup){
    ref_up=refup;
    forward = (lookat - position).normalized();
    right = forward.cross(ref_up).normalized();
    up = right.cross(forward).normalized();
};

Camera::Camera(Vec3f pos,Vec3f lookat,float new_fov,Vec3f refup){
    position=pos;
    fov=new_fov;
    ref_up=refup;
    forward = (lookat - position).normalized();
    right = forward.cross(ref_up).normalized();
    up = right.cross(forward).normalized();
};

void Camera::shotimage(Scene& scene){
    //todo: #1 implement a scene shot function.
};