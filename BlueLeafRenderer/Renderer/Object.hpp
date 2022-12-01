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

enum ObjectType {
    Empty,
    Sphere
};

struct ObjectInfo {
    ObjectType type;
    Point3 point_of_hit;
    Vector3 surface_normal;
    Ray ray_of_hit;
    Material material;
    double t;
    bool front_face;
};

class Object {
public:
    // General attributes used by all objects
    ObjectInfo info;
    AABB bounding_box;
    Point3 center;
private:
    // Attributes specific to spheres (other object)
    // specific attributes will also go here
    double radius;
public:
    Object();
    // Used only for spheres
    Object(Point3 center, double radius, Material& material);
    
    bool hit(const Ray& ray, ObjectInfo& info, double t_min, double t_max);
private:
    void set_surface_normal(const Ray& ray, const Vector3& outward_normal);
    // Used only for sphere
    bool sphere_hit(const Ray& ray, ObjectInfo& info, double t_min, double t_max);
    AABB sphere_bounding_box() const;
};

#endif /* Object_hpp */
