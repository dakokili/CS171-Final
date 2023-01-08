#include "geometry.h"
#include <algorithm>
#include <utility>
#include <fstream>
TriangleMesh::TriangleMesh(std::string filename,float scale,Vec3f pos){
  std::ifstream file(filename);
  std::string buf;
  Vec3f buf2;
  bool flag=false;
  char buf3[100];
  int buf4[6];
  while(!file.eof()){
    file>>buf;
    if(buf=="vn"){
      file>>buf2[0]>>buf2[1]>>buf2[2];
      normals.push_back(buf2);
    }
    else if(buf=="v"){
      file>>buf2[0]>>buf2[1]>>buf2[2];
      vertices.push_back(buf2*scale+pos);
    }
    else if(buf=="f"){
      file.getline(buf3,100);
      bool state=false;
      int k=0;
      for(int i=0;i<100;i++){
        if(buf3[i]=='\0') break;
        if((buf3[i]>=48)&&(buf3[i]<58)){
          if(!state){
            state=true;
            buf4[k]=buf3[i]-'0';
          }
          else{
            buf4[k]=buf4[k]*10+buf3[i]-'0';
          }
        }
        else{
          if(state){
            k++;
            state=false;
          }
        }
      }
      v_indices.push_back(buf4[0]-1);
      v_indices.push_back(buf4[2]-1);
      v_indices.push_back(buf4[4]-1);
      n_indices.push_back(buf4[1]-1);
      n_indices.push_back(buf4[3]-1);
      n_indices.push_back(buf4[5]-1);
    }
  }
  file.close();
  buildBVH();
}

bool TriangleMesh::intersect(openvdb::math::Ray<float>&ray, std::vector<Interaction> &interactions) const {
  Interaction newinteraction;
  bvhHit(bvh, newinteraction, ray);
  if(newinteraction.type==Interaction::GEOMETRY) interactions.push_back(newinteraction);
  return (newinteraction.type==Interaction::GEOMETRY);
}

void TriangleMesh::buildBVH() {
  // TODO: your implementation
  std::vector<int> t_idx;
  for(int i=0;i<v_indices.size()/3;i++) t_idx.push_back(i);
  build(bvh,t_idx);
  std::cout<<bvh->aabb.getDist(0)<<' '<<bvh->aabb.getDist(1)<<' '<<bvh->aabb.getDist(2)<<"\n";
}

float TriangleMesh::sortrelative(int tid,int dim){
  return vertices[v_indices[tid*3]][dim]+vertices[v_indices[tid*3+1]][dim]+vertices[v_indices[tid*3+1]][dim];
}

void TriangleMesh::sort(std::vector<int>& t_idx,int dim){
  std::vector<float> t(t_idx.size());
  std::vector<int> t2(t_idx.size());
  for(int i=0;i<t.size();i++) t[i]=sortrelative(t_idx[i],dim);
  for(int i=0;i<t.size();i++) t2[i]=i;
  std::sort(t2.begin(),t2.end(),[&t](int &a,int &b){return t[a]<t[b];});
  for(int i=0;i<t.size();i++) t2[i]=t_idx[t2[i]];
  //for(int i=0;i<t.size();i++) std::cout<<t2[i]<<' ';
  t_idx.swap(t2);
  //std::cout<<t.size()<<"\n";
}

void TriangleMesh::build(BVHNode*& node, std::vector<int>& t_idx){
    if(!t_idx.size()) return;
    else{
      node=new BVHNode;
      if(t_idx.size()==1){
        node->aabb=AABB(vertices[v_indices[t_idx[0]*3]],vertices[v_indices[t_idx[0]*3+1]],vertices[v_indices[t_idx[0]*3+2]]);
        node->triangles=t_idx[0];
      }
      else{
        int dim=0;
        node->aabb=AABB(vertices[v_indices[t_idx[0]*3]],vertices[v_indices[t_idx[0]*3+1]],vertices[v_indices[t_idx[0]*3+2]]);
        for(int i=1;i<t_idx.size();i++){
          node->aabb=AABB(node->aabb,AABB(vertices[v_indices[t_idx[i]*3]],vertices[v_indices[t_idx[i]*3+1]],vertices[v_indices[t_idx[i]*3+2]]));
        }
        for(int i=0;i<3;i++){
          if(node->aabb.getDist(i)>node->aabb.getDist(dim)) dim=i;
        }
        sort(t_idx,dim);
        std::vector<int> left,right;
        for(int i=0;i<(t_idx.size()+1)/2;i++){
          left.push_back(t_idx[i]);
        }
        for(int i=(t_idx.size()+1)/2;i<t_idx.size();i++){
          right.push_back(t_idx[i]);
        }
        build(node->left,left);
        build(node->right,right);
        node->aabb=AABB(node->left->aabb,node->right->aabb);
      }
    }
}

bool TriangleMesh::intersectOneTriangle(openvdb::math::Ray<float>&ray,
                                        Interaction &interaction,
                                        const Vec3i &v_idx,
                                        const Vec3i &n_idx) const {
  Vec3f v0 = vertices[v_idx[0]];
  Vec3f v1 = vertices[v_idx[1]];
  Vec3f v2 = vertices[v_idx[2]];
  Vec3f direction=Vec3f(ray.dir()[0],ray.dir()[1],ray.dir()[2]);
  Vec3f origin=Vec3f(ray.eye()[0],ray.eye()[1],ray.eye()[2]);
  Vec3f v0v1 = v1 - v0;
  Vec3f v0v2 = v2 - v0;
  Vec3f pvec = direction.cross(v0v2);
  float det = v0v1.dot(pvec);

  float invDet = 1.0f / det;

  Vec3f tvec = origin - v0;
  float u = tvec.dot(pvec) * invDet;
  if (u < 0 || u > 1) return false;
  Vec3f qvec = tvec.cross(v0v1);
  float v = direction.dot(qvec) * invDet;
  if (v < 0 || u + v > 1) return false;
  float t = v0v2.dot(qvec) * invDet;
  if (t < ray.t0() || t > ray.t1()) return false;

  interaction.dist = t;
  interaction.pos = Vec3f(ray(t)[0],ray(t)[1],ray(t)[2]);
  interaction.normal=normals[n_idx[0]]+u*normals[n_idx[1]]+v*normals[n_idx[2]];
  interaction.type = Interaction::Type::GEOMETRY;
  return true;
}

void TriangleMesh::bvhHit(BVHNode *p, Interaction &interaction,
                          openvdb::math::Ray<float>&ray) const {
  // TODO: traverse through the bvh and do intersection test efficiently.
  if(!p) return;
  if(!p->aabb.intersect(ray)) return;
  if(p->triangles!=-1){
    Interaction temp;
    if (intersectOneTriangle(ray,temp,Vec3i(v_indices[p->triangles*3],v_indices[p->triangles*3+1],v_indices[p->triangles*3+2]),\
    Vec3i(n_indices[p->triangles*3],n_indices[p->triangles*3+1],n_indices[p->triangles*3+2])) && (temp.dist < interaction.dist)) {
        interaction = temp;
    }
  }
  else{
    bvhHit(p->left,interaction,ray);
    bvhHit(p->right,interaction,ray);
  }
}