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
    Material object_material;
    double t;
    bool front_face;
private:
    // Attributes specific to spheres (other object)
    // specific attributes will also go here
    Point3 center_start;
    Point3 center_end;
    double time_start;
    double time_end;
    double radius;
public:
    Object();
    // Used only for spheres
    Object(Point3 center_start, double radius, Material& object_material);
    Object(Point3 center_start, Point3 center_end, double time_start, double time_end, double radius, Material& object_material);
    
    bool hit(const Ray& ray, double t_min, double t_max);
    void calculate_hit();
private:
    void set_surface_normal(const Ray& ray, const Vector3& outward_normal);
    // Used only for sphere
    bool sphere_hit(const Ray& ray, double t_min, double t_max);
    void sphere_calculate_hit();
    Point3 center(double time) const;
};

#endif /* Object_hpp */
