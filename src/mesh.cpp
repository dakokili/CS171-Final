#include <mesh.h>
#include <utils.h>
#include <fstream>
#include <vector>
#include <sstream>
Mesh::Mesh(const std::string &path) { loadDataFromFile(getPath(path)); }

/**
 * TODO: implement load mesh data from file here
 * you need to open file with [path]
 *
 * File format
 *
 *
 * For each line starting with 'v' contains 3 floats, which
 * represents the position of a vertex
 *
 * For each line starting with 'n' contains 3 floats, which
 * represents the normal of a vertex
 *
 * For each line starting with 'f' contains 6 integers
 * [v0, n0, v1, n1, v2, n2], which represents the triangle face
 * v0, n0 means the vertex index and normal index of the first vertex
 * v1, n1 means the vertex index and normal index of the second vertex
 * v2, n2 means the vertex index and normal index of the third vertex
 */
void Mesh::loadDataFromFile(const std::string &path) {
    /**
     * path stands for the where the object is storaged
     * */
  std::string code;
  std::ifstream infile(getPath(path));
  if(infile.is_open()) {
    std::string line;
    vec3 nowdata={0.0,0.0,0.0};
    int face[6];
    Vertex avertex;
    while (infile>>line) {
        if(line=="v"){
          infile>>nowdata[0]>>nowdata[1]>>nowdata[2];
          vertices.push_back(nowdata);
        }
        else if(line=="n"){
          infile>>nowdata[0]>>nowdata[1]>>nowdata[2];
          normals.push_back(nowdata);
        }
        else if(line=="f"){
          infile>>face[0]>>face[3]>>face[1]>>face[4]>>face[2]>>face[5];
          for(int i=0;i<3;i++){
            avertex.normal=normals[face[i+3]];
            avertex.position=vertices[face[i]];
            combinedvertices.push_back(avertex);
          }
        }
      }
  }
  infile.close();
}

Mesh::~Mesh(){
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1,&ebo);
};

void Mesh::drawprepare(){
  glGenVertexArrays(1,&vao);
  glGenBuffers(1,&vbo);
  glGenBuffers(1,&ebo);
  
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glBufferData(GL_ARRAY_BUFFER,combinedvertices.size()*sizeof(Vertex),&combinedvertices[0],GL_STATIC_DRAW);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(sizeof(vec3)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
};

/**
 * TODO: implement your draw code here
 */
void Mesh::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,combinedvertices.size());
    glBindVertexArray(0);
}