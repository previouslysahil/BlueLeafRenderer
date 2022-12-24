//
//  Sphere.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/2/22.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include "Object.hpp"

class Sphere: public Object {
public:
    double radius;
public:
    Sphere();
    Sphere(Point3 center, double radius, Material* material);
    
    virtual bool hit(const Ray& ray, ObjectInfo& info, double t_min, double t_max) override;
    virtual AABB make_bounding_box() const override;
private:
    void set_uv(const Point3& point, double& u, double& v);
};

#endif /* Sphere_hpp */
