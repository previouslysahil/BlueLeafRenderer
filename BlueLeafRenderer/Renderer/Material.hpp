//
//  Material.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/26/22.
//

#ifndef Material_hpp
#define Material_hpp

#include "Color.hpp"
#include "Ray.hpp"
#include "Texture.hpp"
#include "Object.hpp"

struct ObjectInfo; // Circular dependency between Object.hpp and Material.hpp (we need ObjectInfo but Object.hpp needs Material)

class Material {
public:
    Texture* albedo;
public:
    Material();
    Material(Texture* albedo);
    virtual ~Material() = default;
    
    virtual bool scatter(const Ray& ray, const ObjectInfo& info, Color& attenuation, Ray& scattered_ray) = 0;
    virtual Color emitted(double u, double v, const Point3& point_of_hit) const;
};

class Lambertian: public Material {
public:
    Lambertian(Texture* albedo);
    virtual bool scatter(const Ray& ray, const ObjectInfo& info, Color& attenuation, Ray& scattered_ray) override;
};

class Metal: public Material {
private:
    double roughness;
public:
    Metal(Texture* albedo, double roughness);
    virtual bool scatter(const Ray& ray, const ObjectInfo& info, Color& attenuation, Ray& scattered_ray) override;
};

class Dielectric: public Material {
private:
    double index_of_refraction;
public:
    Dielectric(Texture* albedo, double index_of_refraction);
    virtual bool scatter(const Ray& ray, const ObjectInfo& info, Color& attenuation, Ray& scattered_ray) override;
};

class DiffuseLight: public Material  {
public:
    DiffuseLight(Texture* a);

    virtual bool scatter(const Ray& ray, const ObjectInfo& info, Color& attenuation, Ray& scattered_ray) override;
    virtual Color emitted(double u, double v, const Point3& point_of_hit) const override;
public:
    Texture* emit;
};

#endif /* Material_hpp */
