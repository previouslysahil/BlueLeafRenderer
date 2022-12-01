//
//  AABB.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/29/22.
//

#include "AABB.hpp"

#include <algorithm>

AABB::AABB(): minimum(), maximum() {}

AABB::AABB(const Point3& minimum, const Point3& maximum): minimum(minimum), maximum(maximum) {}

/// This function checks whether a ray intersects our AABB at all three axses
/// in the t interval range passed
/// - Parameters:
///   - ray: The ray shot towards our bounding box
///   - t_min: The minimum t value our ray must have
///   - t_max: The maximum t value our ray must have
bool AABB::hit(const Ray& ray, double t_min, double t_max) const {
    for (int axis = 0; axis < 3; axis++) {
        double invD = 1.0f / ray.direction[axis];
        double t0 = (minimum[axis] - ray.origin[axis]) * invD;
        double t1 = (maximum[axis] - ray.origin[axis]) * invD;
        if (invD < 0.0f) {
            std::swap(t0, t1);
        }
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min) {
            return false;
        }
    }
    return true;
}
