#include "defines.h"
#include "scene.h"
#include "camera.h"

int main(){
    Scene scene;
    scene.loadgridfromfile("assets/multi-res big.vdb");
    Camera camera(Vec3f(-10,10,-10),Vec3f(0,0,0),90.0f);
    camera.shotimage(scene);
    return 0;
}