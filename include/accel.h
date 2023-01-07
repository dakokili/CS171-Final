#ifndef ACCEL_H_
#define ACCEL_H_

#include "defines.h"
#include <openvdb/math/Ray.h>

struct AABB {
  // the minimum and maximum coordinate for the AABB
  Vec3f low_bnd;
  Vec3f upper_bnd;
  /// test intersection with given ray.
  /// ray distance of entrance and exit point are recorded in t_in and t_out
  AABB() : low_bnd(0, 0, 0), upper_bnd(0, 0, 0) {}
  AABB(Vec3f low, Vec3f upper) : low_bnd(low), upper_bnd(upper) {}
  /// construct an AABB from three vertices of a triangle.
  AABB(const Vec3f &v1, const Vec3f &v2, const Vec3f &v3);
  /// Construct AABB by merging two AABBs
  AABB(const AABB &a, const AABB &b);
  bool intersect(openvdb::math::Ray<float>&ray);
  /// Get the AABB center
  [[nodiscard]] Vec3f getCenter() const { return (low_bnd + upper_bnd) / 2; }
  /// Get the length of a specified side on the AABB
  [[nodiscard]] float getDist(int dim) const { return upper_bnd[dim] - low_bnd[dim]; }
  /// Check whether the AABB is overlapping with another AABB
  [[nodiscard]] bool isOverlap(const AABB &other) const;
};

struct BVHNode {
  BVHNode *left;
  BVHNode *right;
  // bounding box of current node.
  AABB aabb;
  // index of triangles in current BVH leaf node.
  int triangles=-1;
};

// You may need to add your code for BVH construction here.

#endif //ACCEL_H_