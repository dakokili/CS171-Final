#include "grid.h"

Grid::Grid(openvdb::Vec3SGrid::Ptr grid){
    for(auto metadata=grid->beginMeta();metadata!=grid->endMeta();++metadata){
        if(metadata->first=="dx") dx=grid->metaValue<double>(metadata->first);
        if(metadata->first=="origin"){
            origin[0]=grid->metaValue<openvdb::Vec3d>(metadata->first)[0];
            origin[1]=grid->metaValue<openvdb::Vec3d>(metadata->first)[1];
            origin[2]=grid->metaValue<openvdb::Vec3d>(metadata->first)[2];
        }
    }
    Cell acell;
    openvdb::Vec3i coord;
    openvdb::Vec3f value;
    for(auto celldata=grid->cbeginValueOn();celldata;++celldata){
        coord=celldata.getCoord().asVec3i();
        value=celldata.getValue();
        acell.coordinate[0]=coord[0];
        acell.coordinate[1]=coord[1];
        acell.coordinate[2]=coord[2];
        acell.velocity[0]=value[0];
        acell.velocity[1]=value[1];
        acell.velocity[2]=value[2];
        acell.speed=acell.velocity.norm();
        cells.push_back(acell);
    }
    voxel_count=cells.size();
};