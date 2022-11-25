//
//  Sphere.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include "Ray.hpp"
#include "Vector3.hpp"

class Sphere {
public:
    Point3 point_of_hit;
    Vector3 surface_normal;
    Ray ray_of_hit;
    double t;
    bool front_face;
    
    Point3 center;
    double radius;
    
public:
    Sphere();
    Sphere(Point3 center, double radius);
    
    bool hit(const Ray& ray, double t_min, double t_max);
    void calculate_hit();
private:
    void set_surface_normal(const Ray& ray, const Vector3& outward_normal);
};

#endif /* Sphere_hpp */
