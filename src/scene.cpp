#include "scene.h"
#include "utils.h"
#include <algorithm>
#include <openvdb/tools/Interpolation.h>

void Scene::loadgridfromfile(std::string filepath){
    openvdb::initialize();
    openvdb::io::File file(getPath(filepath));
    file.open();
    openvdb::GridPtrVecPtr sourcegrids = file.getGrids();
    for(auto basegrid : *sourcegrids){
        basegrid->print();
        openvdb::Vec3SGrid::Ptr sourcegrid = openvdb::gridPtrCast<openvdb::Vec3SGrid>(basegrid);
        vdbgrids.push_back(sourcegrid->deepCopy());
        //grids.push_back(Grid(sourcegrid));
    }
    file.close();
};

void Scene::loadobjectfromfile(std::string filepath,float scale,Vec3f pos){
    objects.push_back(TriangleMesh(getPath(filepath),scale,pos));
};

void Scene::intersect(openvdb::math::Ray<float>& ray, std::vector<Interaction>& interactions){
    for(auto& object:objects){
        object.intersect(ray,interactions);
    }
    for(auto vdbgrid: vdbgrids){
        float scale=vdbgrid->voxelSize()[0];
        auto acc = vdbgrid->getAccessor();
        auto dda=openvdb::math::DDA(ray.worldToIndex(*vdbgrid));
        Interaction interaction;
        openvdb::Vec3f pos;
        interaction.type=interaction.VOXEL;
        while(dda.step()){
            if(acc.isValueOn(dda.voxel())){
                interaction.dist=dda.time()*scale;
                pos=ray.worldToIndex(*vdbgrid)(dda.time());
                //std::cout<<pos;
                //std::cout<<dda.voxel()<<'\n';
                interaction.scale=scale;
                float data[2][2][2];
                openvdb::Coord ijk(openvdb::tools::local_util::floorVec3(pos));
                pos=ray(interaction.dist);
                interaction.pos[0]=pos[0];
                interaction.pos[1]=pos[1];
                interaction.pos[2]=pos[2];
                data[0][0][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][0][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[1]+=1;
                data[0][1][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=1;
                data[0][1][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[0]+=1;
                ijk[1]-=1;
                data[1][0][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][0][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[1]+=1;
                data[1][1][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=1;
                data[1][1][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                openvdb::Vec3R uvw=openvdb::Vec3R(pos)-openvdb::tools::local_util::floorVec3(pos);
                auto _interpolate = [](float a, float b, float weight)
                {
                    const auto temp = (b - a) * weight;
                    return (a + temp);
                };
                interaction.value=_interpolate(
                    _interpolate(
                        _interpolate(data[0][0][0], data[0][0][1], uvw[2]),
                        _interpolate(data[0][1][0], data[0][1][1], uvw[2]),
                        uvw[1]),
                    _interpolate(
                        _interpolate(data[1][0][0], data[1][0][1], uvw[2]),
                        _interpolate(data[1][1][0], data[1][1][1], uvw[2]),
                        uvw[1]),
                uvw[0]);
                interactions.push_back(interaction);
            }
        }
    }
    std::sort(interactions.begin(),interactions.end(),[](Interaction& a,Interaction& b){
        if(a.dist==b.dist) return a.scale>b.scale;
        return a.dist<b.dist;});
};