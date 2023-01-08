#include "camera.h"
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#include "interaction.h"
#include <omp.h>
#include <algorithm>

Image::Image(){
    resolution.x() = 1500;
    resolution.y() = 1000;
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
    Sampler sampler;
    omp_set_nested(1);
    omp_set_dynamic(0);
#pragma omp parallel for schedule(guided) num_threads(12) private(j,sampler)
    for(i=0;i<image.resolution.y();i++){
#pragma omp atomic
        ++count;
        sampler.setSeed(count);
        printf("\r%.02f%%", count * 100.0 / image.resolution.y());
        for(j=0;j<image.resolution.x();j++) {
            Vec3i ans(0,0,0);
            std::vector<Interaction> interactions;
            for(int k=0;k<SPP;k++){
                interactions.swap(std::vector<Interaction>());
                scene.intersect(generateray(float(j)+sampler.get1D(),float(i)+sampler.get1D()),interactions);
                ans+=transferfunction(interactions);
            }
            image.setpixel(j,image.resolution.y()-1-i,ans/SPP);
        }
    }
    image.writefile("result.png");
};

openvdb::math::Ray<float> Camera::generateray(float x,float y){
    Vec3f direction=forward+tanf(fov*PI/360.0f)*up*(y-float(image.resolution.y()/2))/float(image.resolution.y()/2)+tanf(fov*PI/360.0f)*right*(x-float(image.resolution.x()/2))/float(image.resolution.y()/2);
    return openvdb::math::Ray<float>(openvdb::Vec3s(position.x(),position.y(),position.z()),
    openvdb::Vec3s(direction.x(),direction.y(),direction.z()),
    RAY_DEFAULT_MIN,RAY_DEFAULT_MAX);
};

Vec3i Camera::transferfunction(std::vector<Interaction>& interactions){
    if(interactions.size()==0) return{0,0,0};
    else
    {
        int length = interactions.size();
        Vec3f s={0,0,0};
        float t=.03f;
        int count=0;
        bool objhit=false;
        auto _ratio= [](int counter,float t){
            auto temp=0.0f;
            for(int i=0;i<counter;i++) temp=temp*(1-t)+t;
            return 1.f/temp;
        };
        for(int i=0 ; i < length ; i++)
        {
            if(interactions[i].type==Interaction::VOXEL){
                interactions[i].value*=6500.f;
                if(interactions[i].value>1.f){
                    count++;
                    s*=1-t;
                    s+=t*Vec3f(1.f/interactions[i].value,1.f-1.f/interactions[i].value,0);
                }
            }
            else if(interactions[i].type==Interaction::GEOMETRY){
                if(count>0) s*=_ratio(count,t);
                else s+=Vec3f{.8f,.3f,.8f}*(.4f+.2f*std::max(0.f,interactions[i].normal.dot(Vec3f(0,10,0)-interactions[i].pos)));
                objhit=true;
                break;
            }
        }
        if(!objhit&&count>0) s*=_ratio(count,t);
        return converttoRGB(s);
    }
}