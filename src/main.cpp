#include <utils.h>
int main(int argc, char* argv[]){
    openvdb::io::File file(getPath("assets/single-res small.vdb",5));
    std::cout<<file.filename();
    return 0;
}