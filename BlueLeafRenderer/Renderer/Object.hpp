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

enum ObjectType {
    Empty,
    Sphere
};

class Object {
public:
    // General attributes used by all objects
    ObjectType type;
    Point3 point_of_hit;
    Vector3 surface_normal;
    Ray ray_of_hit;
    double t;
    bool front_face;
private:
    // Attributes specific to spheres (other object)
    // specific attributes will also go here
    Point3 center;
    double radius;
public:
    Object();
    // Used only for spheres
    Object(Point3 center, double radius);
    
    bool hit(const Ray& ray, double t_min, double t_max);
    void calculate_hit();
private:
    void set_surface_normal(const Ray& ray, const Vector3& outward_normal);
    // Used only for sphere
    bool hit_sphere(const Ray& ray, double t_min, double t_max);
};

#endif /* Object_hpp */
