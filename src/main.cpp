#include "utils.h"

int main()
{
    std::vector<Grid> grids;
    openvdb::initialize();
    File file(getPath("/assets/multi-res small.vdb"));
    file.open();
    openvdb::GridPtrVecPtr sourcegrids = file.getGrids();
    for(auto basegrid : *sourcegrids){
        basegrid->print();
        openvdb::Vec3SGrid::Ptr sourcegrid = openvdb::gridPtrCast<openvdb::Vec3SGrid>(basegrid);
        grids.push_back(Grid(sourcegrid));
    }
    file.close();

    
}