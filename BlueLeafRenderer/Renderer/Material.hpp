//
//  Material.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/26/22.
//

#ifndef Material_hpp
#define Material_hpp

#include "Color.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"

class Material {
public:
    Color albedo;
public:
    Material();
    Material(Color albedo);
    virtual ~Material() = default;
    
    virtual bool scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray) = 0;
};

class Lambertian: public Material {
public:
    Lambertian(Color albedo);
    virtual bool scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray) override;
};

class Metal: public Material {
private:
    double roughness;
public:
    Metal(Color albedo, double roughness);
    virtual bool scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray) override;
};

class Dielectric: public Material {
private:
    double index_of_refraction;
public:
    Dielectric(Color albedo, double index_of_refraction);
    virtual bool scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray) override;
};


#endif /* Material_hpp */
