#ifndef _MESH_H_
#define _MESH_H_
#include "defines.h"
#include <vector>

struct Vertex {
  vec3 position;
  vec3 normal;
};

class Mesh {
 public:
  Mesh(const std::string &path);
  ~Mesh();
  void draw() const;
  void drawprepare();
 private:
  //  mesh data
  std::vector<vec3> vertices;
  std::vector<vec3> normals;
  std::vector<Vertex> combinedvertices;
  GLuint vao,vbo,ebo;
  void loadDataFromFile(const std::string &path);
};
#endif