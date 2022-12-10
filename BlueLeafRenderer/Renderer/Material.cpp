//
//  Material.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/26/22.
//

#include "Material.hpp"
#include "LinearAlgebra.hpp"

/// Initalizes albedo to zero
Material::Material(): albedo() {}

/// Initalizes albedo
/// - Parameters:
///   - albedo: The color of the material
Material::Material(Color albedo): albedo(albedo) {}

/// Initalizes albedo
/// - Parameters:
///   - albedo: The color of the lambertian
Lambertian::Lambertian(Color albedo): Material(albedo) {}

/// Initalizes albedo and roughness
/// - Parameters:
///   - albedo: The color of the metal
///   - roughness: The roughness of our metal
Metal::Metal(Color albedo, double roughness): Material(albedo), roughness(roughness < 1 ? roughness : 1) {}

/// Initalizes albedo and roughness
/// - Parameters:
///   - albedo: The color of the metal
///   - index_of_refraction: The roughness of our metal
Dielectric::Dielectric(Color albedo, double index_of_refraction): Material(albedo), index_of_refraction(index_of_refraction) {}

/// Scatters using lambertian scattering which gets a random point on the unit sphere
/// of our point of hit and uses this to create the vector directino of our scatter
/// - Parameters:
///   - ray: The incoming ray that we will use to create our new scattered ray
///   - point_of_hit: The point our incoming ray hit our material's object at
///   - surface_normal: The surface normal of the point of hit
///   - front_face: The direction of our surface normal (in/ out)
///   - attenuation: The color that we will assign based on the materials color
///   at this point
///   - scattered_ray: The new ray that we will create of the above params
bool Lambertian::scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray) {
    // This get the vector S that is cast from our point of hit's
    // surface normal to a point on the unit sphere of point of
    // hit + surface normal
    Vector3 scatter_direction = surface_normal + random_unit_vector();
    // Change this scatter direction for zero since it may cause NANs
    if (scatter_direction.near_zero()) {
        scatter_direction = surface_normal;
    }
    // Now we cast our ray in this new direction
    scattered_ray = Ray(point_of_hit, scatter_direction);
    // Pass the color of the material back for proper coloring
    attenuation = albedo;
    // Lambertian always scatters
    return true;
}

/// Scatters by reflecting the incoming ray across the normal at the point of hit and
/// then randomly modulates this reflected ray using a random point in the unit sphere
/// of the reflected ray to add roughness based on the provided roughness value
/// - Parameters:
///   - ray: The incoming ray that we will use to create our new scattered ray
///   - point_of_hit: The point our incoming ray hit our material's object at
///   - surface_normal: The surface normal of the point of hit
///   - front_face: The direction of our surface normal (in/ out)
///   - attenuation: The color that we will assign based on the materials color
///   at this point
///   - scattered_ray: The new ray that we will create of the above params
bool Metal::scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray) {
    // For metal materials the direction of our new ray
    // is a reflection across our surface normal
    Vector3 reflected = reflect(unit_vector(ray.direction), surface_normal);
    // Now we cast our ray in this new direction, we distort the
    // true reflection if we want some roughness using a random
    // in our unit sphere of the reflected ray
    scattered_ray = Ray(point_of_hit, reflected + random_in_unit_sphere() * roughness);
    // Pass the color of the material back for proper coloring
    attenuation = albedo;
    // We only scatter our ray if the direction of our scattering
    // is similar to the direction of our surface normal
    return (dot(scattered_ray.direction, surface_normal) > 0);
}

/// Scatters by reflecting a ray if there is no solution to snells law and refracting
/// it if there is a solution to snells law, a solution to snells law will not exist
/// when the refracted index of the material is higher than the outside element
/// also uses schlicks approximation to reflect if viewing at a steep angle
/// - Parameters:
///   - ray: The incoming ray that we will use to create our new scattered ray
///   - point_of_hit: The point our incoming ray hit our material's object at
///   - surface_normal: The surface normal of the point of hit
///   - front_face: The direction of our surface normal (in/ out)
///   - attenuation: The color that we will assign based on the materials color
///   at this point
///   - scattered_ray: The new ray that we will create of the above params
bool Dielectric::scatter(const Ray& ray, const Point3& point_of_hit, const Vector3& surface_normal, const bool& front_face, Color& attenuation, Ray& scattered_ray) {
    // Invert refractio ratio if normal direction is inward
    double refraction_ratio = front_face ? (1.0 / index_of_refraction) : index_of_refraction;
    
    // Calculate our refracted ray
    Vector3 unit_direction = unit_vector(ray.direction);
    double cos_theta = fmin(dot(-unit_direction, surface_normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    
    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector3 direction;
    // We decide if we refract based on whether or not we can solve snells law
    // and we also use schlicks approximation which determines when to reflect
    // based on the angle of our ray hitting our object (water more mirror like
    // like at steep angle)
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
        direction = reflect(unit_direction, surface_normal);
    } else {
        direction = refract(unit_direction, surface_normal, refraction_ratio);
    }
    // Cast our ray using the reflect or refract based on our
    // refraction decision
    scattered_ray = Ray(point_of_hit, direction);
    // Pass the color of the material back for proper coloring
    attenuation = albedo;
    // Always scatters
    return true;
}
