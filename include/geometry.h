#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "interaction.h"
#include "accel.h"
#include "defines.h"

#include <optional>

class TriangleMesh {
 public:
  TriangleMesh() = default;
  TriangleMesh(std::string filename,float scale,Vec3f pos);
  bool intersect(openvdb::math::Ray<float>&ray, std::vector<Interaction> &interactions) const;

  void buildBVH();
  void build(BVHNode*& node,std::vector<int>& t_idx);
  void sort(std::vector<int>& t_idx,int dim);
  float sortrelative(int tid, int dim);
 private:
  bool intersectOneTriangle(openvdb::math::Ray<float>&ray, Interaction &interaction, const Vec3i& v_idx, const Vec3i& n_idx) const;
  void bvhHit(BVHNode *p, Interaction &interaction,
              openvdb::math::Ray<float>&ray) const;

  BVHNode* bvh;
  std::vector<Vec3f> vertices;
  std::vector<Vec3f> normals;
  std::vector<int> v_indices;
  std::vector<int> n_indices;
};

#endif // GEOMETRY_H_
