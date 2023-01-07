#ifndef _SCENE_H_
#define _SCENE_H_
#include "defines.h"
#include "geometry.h"
#include <openvdb/openvdb.h>
#include <openvdb/math/Ray.h>
#include <openvdb/math/DDA.h>
#include "interaction.h"
class Scene{
public:
    std::vector<openvdb::Vec3SGrid::Ptr> vdbgrids;
    std::vector<TriangleMesh> objects;
    void loadgridfromfile(std::string filepath);
    void loadobjectfromfile(std::string filepath,float scale,Vec3f pos);
    void intersect(openvdb::math::Ray<float>& ray,std::vector<Interaction>& interactions);
};
#endif