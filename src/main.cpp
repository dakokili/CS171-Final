#include "defines.h"
#include "scene.h"
#include "camera.h"

int main(){
    Scene scene;
    scene.loadgridfromfile("assets/multi-res small.vdb");
    Camera camera;
    camera.shotimage(scene);
    return 0;
}