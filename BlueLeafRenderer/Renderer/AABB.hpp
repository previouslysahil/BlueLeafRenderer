//
//  AABB.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/29/22.
//

#ifndef AABB_hpp
#define AABB_hpp

#include "Vector3.hpp"
#include "Ray.hpp"

class AABB {
public:
    Point3 minimum;
    Point3 maximum;
public:
    AABB();
    AABB(const Point3& minimum, const Point3& maximum);
    
    bool hit(const Ray& ray, double t_min, double t_max) const;
};

inline AABB surrounding_box(const AABB& box0, const AABB& box1) {
    Point3 small(fmin(box0.minimum.x, box1.minimum.x), fmin(box0.minimum.y, box1.minimum.y), fmin(box0.minimum.z, box1.minimum.z));
    Point3 big(fmax(box0.maximum.x, box1.maximum.x), fmax(box0.maximum.y, box1.maximum.y), fmax(box0.maximum.z, box1.maximum.z));
    return AABB(small, big);
}

inline int longest_axis(const Vector3& length) {
    int axis = 0;
    if (length.y > length.x) axis = 1;
    if (length.z > length.y) axis = 2;
    return axis;
}

#endif /* AABB_hpp */
