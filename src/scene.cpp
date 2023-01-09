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
        openvdb::Vec3SGrid::Ptr sourcegrid = openvdb::gridPtrCast<openvdb::Vec3SGrid>(basegrid);
        openvdb::FloatGrid::Ptr newgrid = openvdb::FloatGrid::create();
        auto acc=sourcegrid->getAccessor();
        auto acc2=newgrid->getAccessor();
        for(auto iter=sourcegrid->beginValueOn();iter.test();++iter){
            auto coord=iter.getCoord();
            float newvalue=0;
            if(!velocity){
                auto Qx=(acc.isValueOn(coord+openvdb::Coord(1,0,0))? acc.getValue(coord+openvdb::Coord(1,0,0)):acc.getValue(coord))
                -(acc.isValueOn(coord+openvdb::Coord(-1,0,0))? acc.getValue(coord+openvdb::Coord(-1,0,0)):acc.getValue(coord));
                auto Qy=(acc.isValueOn(coord+openvdb::Coord(0,1,0))? acc.getValue(coord+openvdb::Coord(0,1,0)):acc.getValue(coord))
                -(acc.isValueOn(coord+openvdb::Coord(0,-1,0))? acc.getValue(coord+openvdb::Coord(0,-1,0)):acc.getValue(coord));
                auto Qz=(acc.isValueOn(coord+openvdb::Coord(0,0,1))? acc.getValue(coord+openvdb::Coord(0,0,1)):acc.getValue(coord))
                -(acc.isValueOn(coord+openvdb::Coord(0,0,-1))? acc.getValue(coord+openvdb::Coord(0,0,-1)):acc.getValue(coord));
                newvalue=abs(0.5f*(Qx[0]*Qx[0]+Qy[1]*Qy[1]+Qz[2]*Qz[2])+Qy[0]*Qx[1]+Qz[0]*Qx[2]+Qz[1]*Qy[2]);
            }
            else newvalue=acc.getValue(coord).length();
            acc2.setValue(coord,newvalue);
        }
        newgrid->setTransform(sourcegrid->transformPtr());
        vdbgrids.push_back(newgrid);
        newgrid->print();
        vdbAABB box;
        box.scale=sourcegrid->voxelSize()[0];
        box.bottom=sourcegrid->indexToWorld(openvdb::Coord(0,0,0));
        box.top=sourcegrid->indexToWorld(openvdb::Coord(basegrid->metaValue<openvdb::Vec3i>(openvdb::Name(sourcegrid->META_FILE_BBOX_MAX))));
        std::cout<<box.bottom<<box.top<<'\n';
        vdbAABBboxes.push_back(box);
    }
    file.close();
};

void Scene::loadobjectfromfile(std::string filepath,float scale,Vec3f pos){
    objects.push_back(TriangleMesh(getPath(filepath),scale,pos));
};

void Scene::intersect(openvdb::math::Ray<float>& ray, std::vector<Interaction>& interactions){
    auto _interpolate = [](float a, float b, float c, float d,float u) 
    {
        float temp=0.f;
        if(linearorbspline) temp = b*(1-u)+c*u;
        else temp = (a*(-u*u*u+3*u*u-3*u+1)+b*(3*u*u*u-6*u*u+4)+c*(-3*u*u*u+3*u*u+3*u+1)+d*u*u*u)/6;
        return temp;
    };
    for(auto& object:objects){
        object.intersect(ray,interactions);
    }
    float time=ray.t0();
    std::vector<openvdb::v10_0::FloatGrid::Accessor> acc;
    for(int k=0;k<vdbgrids.size();k++) acc.push_back(vdbgrids[k]->getAccessor());
    while(time<=ray.t1()){
        openvdb::Vec3f pos=ray(time);
        int i=0;
        while(i<vdbgrids.size()){
            if(!vdbAABBboxes[i].isInside(pos)) break;
            i++;
        }
        if(i>0){
            float data[4][4][4];
            Interaction interaction;
            interaction.dist=time;
            interaction.type=interaction.VOXEL;
            label:
            i--;
            openvdb::Vec3f indexpos=vdbgrids[i]->worldToIndex(pos);
            openvdb::Coord ijk(openvdb::tools::local_util::floorVec3(indexpos));
            interaction.scale=vdbAABBboxes[i].scale;
            openvdb::Vec3R uvw=openvdb::Vec3R(indexpos)-openvdb::tools::local_util::floorVec3(indexpos);
            ijk[0]--;ijk[1]--;ijk[2]--;
                for(int x=0;x<4;x++){
                    for(int y=0;y<4;y++){
                        for(int z=0;z<4;z++){
                            openvdb::Coord realijk(ijk[0]+x,ijk[1]+y,ijk[2]+z);
                            if(acc[i].isValueOn(realijk)) data[x][y][z]=velocity? (acc[i].getValue(realijk)):(acc[i].getValue(realijk)*powf(4.f,float(i)));
                            else if(i>0) goto label;
                            else data[x][y][z]=velocity? 1:0;
                        }
                    }
                }
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
                interaction.pos[0]=pos[0];
                interaction.pos[0]=pos[1];
                interaction.pos[0]=pos[2];
                interactions.push_back(interaction);
                time+=vdbAABBboxes[i].scale;
        }
        else{
            break;
        }
    }
    std::sort(interactions.begin(),interactions.end(),[](Interaction& a,Interaction& b){
        if(a.dist==b.dist) return a.scale>b.scale;
        return a.dist<b.dist;});
};