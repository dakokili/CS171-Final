#include "defines.h"
#include "scene.h"
#include "camera.h"

int main(){
    Scene scene;
    scene.loadgridfromfile("assets/multi-res big.vdb");
    scene.loadobjectfromfile("assets/sphere.obj",4.f/3.f,Vec3f(5,2,3));
    Camera camera(Vec3f(5,2,0),Vec3f(5,2,4),90.0f);
    camera.shotimage(scene);
    return 0;
}