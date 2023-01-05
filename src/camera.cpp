#include "camera.h"
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#include "interaction.h"
#include <omp.h>

Image::Image(){
    resolution.x() = 800;
    resolution.y() = 800;
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
    right = ref_up.cross(forward).normalized();
    up = forward.cross(right).normalized();
    image = Image();
};

void Camera::shotimage(Scene& scene){
    int i=0,j=0;
    int count=0;
    omp_set_nested(1);
    omp_set_dynamic(0);
#pragma omp parallel for schedule(guided) num_threads(8) private(j)
    for(i=0;i<image.resolution.y();i++){
#pragma omp atomic
        ++count;
        printf("\r%.02f%%", count * 100.0 / image.resolution.y());
        for(j=0;j<image.resolution.x();j++) {
            std::vector<Interaction> interactions;
            scene.intersect(generateray(j,i),interactions);
            if(j==400&&i==500){
                for(auto inter:interactions) std::cout<<inter.value<<' ';
            }
            image.setpixel(j,image.resolution.y()-1-i,transferfunction(interactions));
        }
    }
    image.writefile("result.png");
};

openvdb::math::Ray<float> Camera::generateray(int x,int y){
    Vec3f direction=forward+tanf(fov*PI/360.0f)*up*(float(y)-400.0f+0.5f)/400.0f+tanf(fov*PI/360.0f)*right*(float(x)-400.0f+0.5f)/400.0f;
    direction.normalize();
    return openvdb::math::Ray<float>(openvdb::Vec3s(position.x(),position.y(),position.z()),
    openvdb::Vec3s(direction.x(),direction.y(),direction.z()),
    RAY_DEFAULT_MIN,RAY_DEFAULT_MAX);
};

Vec3i Camera::transferfunction(std::vector<Interaction>& interactions){
    if(interactions.size()==0) return{0,0,0};
    else //return Vec3i(floor(interactions[0].pos[0]*20),floor(interactions[0].pos[1]*20),floor(interactions[0].pos[2]*20));
    {
        int length = interactions.size();
        Vec3f radiance={0,0,0};
        float t=1;
        float s=0;
        for(int i=0 ; i < length ; i++)
        {
            s = s + interactions[i].value*t;
            t = t * 0.f;
        }
        radiance[2]=s;
        return converttoRGB(radiance);
    }
}