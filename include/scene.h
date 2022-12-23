#ifndef _SCENE_H_
#define _SCENE_H_
#include "defines.h"
#include "grid.h"

class Scene{
public:
    std::vector<Grid> grids;
    void loadgridfromfile(std::string filepath);
};
#endif