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

enum MaterialType {
    None,
    Lambertian,
    Metal,
    Dielectric
};

class Material {
public:
    MaterialType type;
    Color albedo;
private:
    // Used only for metal
    double roughness;
    // Used only for dielectric
    double index_of_refraction;
public:
    Material();
    Material(Color albedo, MaterialType type);
    // Used only for metal
    Material(Color albedo, double roughness);
    // Used only for dielectric
    Material(double index_of_refraction);
    
    bool scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray);
private:
    // Used only for lambertian
    bool lambertian_scatter(const Point3& point_of_hit, const Vector3& surface_normal, Color& attenuation, Ray& scattered_ray);
    // Used only for metal
    bool metal_scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, Color& attenuation, Ray& scattered_ray);
    // Used only for dielectric
    bool dielectric_scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray);
};


#endif /* Material_hpp */
