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
                interaction.dist=dda.next()*scale;
                pos=ray.worldToIndex(*vdbgrid)(dda.time());
                //std::cout<<pos;
                //std::cout<<dda.voxel()<<'\n';
                interaction.pos[0]=pos[0];
                interaction.pos[1]=pos[1];
                interaction.pos[2]=pos[2];
                interaction.scale=scale;
                /*
                float data[2][2][2];
                openvdb::Coord ijk(openvdb::tools::local_util::floorVec3(pos));
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
                */
                openvdb::Vec3R uvw=openvdb::Vec3R(pos)-openvdb::tools::local_util::floorVec3(pos);
                /*
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
                */
               float data[4][4][4];
                openvdb::Coord ijk(openvdb::tools::local_util::floorVec3(pos));
                ijk[0]--;ijk[1]--;ijk[2]--;
                data[0][0][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][0][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][0][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][0][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[0][1][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][1][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][1][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][1][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[0][2][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][2][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][2][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][2][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[0][3][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][3][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][3][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[0][3][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]-=3;ijk[0]+=1;
                data[1][0][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][0][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][0][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][0][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[1][1][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][1][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][1][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][1][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[1][2][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][2][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][2][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][2][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[1][3][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][3][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][3][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[1][3][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]-=3;ijk[0]+=1;
                data[2][0][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][0][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][0][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][0][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[2][1][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][1][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][1][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][1][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[2][2][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][2][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][2][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][2][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[2][3][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][3][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][3][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[2][3][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]-=3;ijk[0]+=1;
                data[3][0][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][0][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][0][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][0][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[3][1][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][1][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][1][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][1][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[3][2][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][2][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][2][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][2][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]-=3;ijk[1]+=1;
                data[3][3][0]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][3][1]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][3][2]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
                ijk[2]+=1;
                data[3][3][3]=(acc.isValueOn(ijk))? acc.getValue(ijk).length():1;
               auto _interpolate = [](float a, float b, float c, float d,float u) 
                {
                    const auto temp = (a*(-u*u*u+3*u*u-3*u+1)+b*(3*u*u*u+6*u*u+4)+c*(-3*u*u*u+3*u*u+3*u+1)+d*u*u*u)/6;
                    return temp;
                };
                interaction.value=_interpolate(
                    _interpolate(
                        _interpolate(data[0][0][0], data[0][0][1], data[0][0][2], data[0][0][3], uvw[2]),
                        _interpolate(data[0][1][0], data[0][1][1], data[0][1][2], data[0][1][3], uvw[2]),
                        _interpolate(data[0][2][0], data[0][2][1], data[0][2][2], data[0][2][3], uvw[2]),
                        _interpolate(data[0][3][0], data[0][3][1], data[0][3][2], data[0][3][3], uvw[2]),
                        uvw[1]),
                    _interpolate(
                        _interpolate(data[1][0][0], data[1][0][1], data[1][0][2], data[1][0][3], uvw[2]),
                        _interpolate(data[1][1][0], data[1][1][1], data[1][1][2], data[1][1][3], uvw[2]),
                        _interpolate(data[1][2][0], data[1][2][1], data[1][2][2], data[1][2][3], uvw[2]),
                        _interpolate(data[1][3][0], data[1][3][1], data[1][3][2], data[1][3][3], uvw[2]),
                        uvw[1]),
                    _interpolate(
                        _interpolate(data[2][0][0], data[2][0][1], data[2][0][2], data[2][0][3], uvw[2]),
                        _interpolate(data[2][1][0], data[2][1][1], data[2][1][2], data[2][1][3], uvw[2]),
                        _interpolate(data[2][2][0], data[2][2][1], data[2][2][2], data[2][2][3], uvw[2]),
                        _interpolate(data[2][3][0], data[2][3][1], data[2][3][2], data[2][3][3], uvw[2]),
                        uvw[1]),
                    _interpolate(
                        _interpolate(data[3][0][0], data[3][0][1], data[3][0][2], data[3][0][3], uvw[2]),
                        _interpolate(data[3][1][0], data[3][1][1], data[3][1][2], data[3][1][3], uvw[2]),
                        _interpolate(data[3][2][0], data[3][2][1], data[3][2][2], data[3][2][3], uvw[2]),
                        _interpolate(data[3][3][0], data[3][3][1], data[3][3][2], data[3][3][3], uvw[2]),
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