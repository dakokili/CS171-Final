#include "scene.h"
#include "utils.h"
#include <algorithm>

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
                //pos=ray(dda.time()*scale);
                pos=vdbgrid->indexToWorld(dda.voxel());
                interaction.pos[0]=pos[0];
                interaction.pos[1]=pos[1];
                interaction.pos[2]=pos[2];
                interaction.scale=scale;
                interaction.value=acc.getValue(dda.voxel()).length();
                interactions.push_back(interaction);
            }
        }
    }
    std::sort(interactions.begin(),interactions.end(),[](Interaction& a,Interaction& b){
        if(a.dist==b.dist) return a.scale>b.scale;
        return a.dist<b.dist;});
};