#ifndef _GRID_H_
#define _GRID_H_
#include "defines.h"
#include <openvdb/openvdb.h>

struct Cell{
        Vec3i coordinate;
        Vec3f velocity;
        double speed;
};

class Grid{
public:
    Grid(openvdb::Vec3SGrid::Ptr grid);
    Vec3f origin={0,0,0};
    double dx=0;
    int voxel_count=0;
    std::vector<Cell> cells;
};
#endif