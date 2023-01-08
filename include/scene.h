#ifndef _SCENE_H_
#define _SCENE_H_
#include "defines.h"
#include "geometry.h"
#include <openvdb/openvdb.h>
#include <openvdb/math/Ray.h>
#include <openvdb/math/DDA.h>
#include "interaction.h"
struct vdbAABB{
    openvdb::Vec3s bottom,top;
    bool isInside(openvdb::Vec3s pos){
        return (pos[0]>=bottom[0])&&(pos[1]>=bottom[1])&&(pos[2]>=bottom[2])
        &&(pos[0]<=top[0])&&(pos[1]<=top[1])&&(pos[2]<=top[2]);
    }
};

class Scene{
public:
    std::vector<openvdb::Vec3SGrid::Ptr> vdbgrids;
    std::vector<vdbAABB> vdbAABBboxes;
    std::vector<TriangleMesh> objects;
    void loadgridfromfile(std::string filepath);
    void loadobjectfromfile(std::string filepath,float scale,Vec3f pos);
    void intersect(openvdb::math::Ray<float>& ray,std::vector<Interaction>& interactions);
};
#endif