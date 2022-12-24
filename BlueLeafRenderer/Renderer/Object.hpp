//
//  Object.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/26/22.
//

#ifndef Object_hpp
#define Object_hpp

#include "Ray.hpp"
#include "Vector3.hpp"
#include "Material.hpp"
#include "AABB.hpp"

class Material;  // Circular dependency between Object.hpp and Material.hpp

struct ObjectInfo {
    Point3 point_of_hit;
    Vector3 surface_normal;
    Ray ray_of_hit;
    Material* material;
    double t;
    double u;
    double v;
    bool front_face;
};

class Object {
public:
    // General attributes used by all objects
    ObjectInfo info;
    Point3 center;
    AABB bounding_box;
public:
    Object();
    Object(Material* material, Point3 center);
    virtual ~Object() = default;
    
    virtual bool hit(const Ray& ray, ObjectInfo& info, double t_min, double t_max) = 0;
    virtual AABB make_bounding_box() const = 0;
    void set_surface_normal(const Ray& ray, const Vector3& outward_normal);
};

#endif /* Object_hpp */
