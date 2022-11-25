//
//  Sphere.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Sphere.hpp"

#include <cmath>

/// Initalizes all values to zeros or false
Sphere::Sphere(): point_of_hit(), surface_normal(), ray_of_hit(), t(0), front_face(false), center(), radius(0) {}

/// Initalizes all values to zeros or false except for center and radius which define the circles location
/// - Parameters:
///   - center: The center of our circle
///   - radius: The radius of our circle
Sphere::Sphere(Point3 center, float radius): point_of_hit(), surface_normal(), ray_of_hit(), t(0), front_face(false), center(center), radius(radius) {}

/// Calculates the hit equation of sphere given by (A + tb - C)(A + tb - C) = r^2
/// by solving for t, less than 0 means no miss, exactly 0 means 1 hit, greater
/// than zero means two solutions, returns the t value along our ray that we
/// hit our sphere or -1.0 if we did not hit our sphere
/// - Parameters:
///   - ray: The ray we cast from the origin point to our viewport
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Sphere::hit(const Ray &ray, float t_min, float t_max) {
    Vector3 oc = ray.origin - center;
    float a = ray.direction.length_squared();
    float half_b = dot(oc, ray.direction);
    float c = oc.length_squared() - radius * radius;
    
    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    float sqrtd = std::sqrt(discriminant);
    
    // Find the nearest t that lies in our range if we did hit
    // our sphere, start with - root
    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        // - Root didnt work try + root
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }
    // Cache our t and ray of hit in case we want to calculate
    // this hit
    t = root;
    ray_of_hit = ray;
    return true;
}

/// This function assumes that t has been set which will always occur if the hit function
/// returns true, assuming yes it will generate the point our ray hit our object at and also
/// generate the surface normal
void Sphere::calculate_hit() {
    point_of_hit = ray_of_hit.at(t);
    Vector3 outward_normal = (point_of_hit - center) / radius;
    set_surface_normal(ray_of_hit, outward_normal);
}

/// Uses the outward normal of our object and the ray direction to determine whether
/// we are inside our oustide the object and as such if we need to change the direction
/// of our surface normal so it always points towards our ray direction, note this decision
/// to always make the ray and surface normal point at each other is a design decision
/// other implementations exist (check them out!)
/// - Parameters:
///   - ray: The ray being cast from our origin to the view port (intersecting with this object
///   at a point t) determined by the hit function
///   - outward_normal: The normal facing out of the object at the point of intersection
///   with our ray
void Sphere::set_surface_normal(const Ray& ray, const Vector3& outward_normal) {
    // Dot product less than 0 implies the ray and outward normal are goin
    // against each other AKA ray is outside object
    front_face = dot(ray.direction, outward_normal) < 0;
    // Based on the direction of our ray to our object/ if we are inside
    // or outside our object we change the surface normal
    surface_normal = front_face ? outward_normal : -outward_normal;
}
