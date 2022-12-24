//
//  Sphere.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/2/22.
//

#include "Sphere.hpp"

Sphere::Sphere(): Object(), radius(0) {}

/// Initalizes all values to zeros or false except for center and radius which
/// define the circles location and sets the object type to sphere, also
/// makes the bounding box of the sphere and set material for sphere
/// USED ONLY FOR SPHERES
/// - Parameters:
///   - center: The center of our circle
///   - radius: The radius of our circle
///   - material: The material of this sphere object
Sphere::Sphere(Point3 center, double radius, Material* material): radius(radius), Object(material, center) {
    bounding_box = make_bounding_box();
}

/// Calculates the hit equation of sphere given by (A + tb - C)(A + tb - C) = r^2
/// by solving for t, less than 0 means no miss, exactly 0 means 1 hit, greater
/// than zero means two solutions, after getting this t value we can calculate
/// our point of hit and surface normal, returns true if we found a t and will
/// pass back our object info for use elsewhere
/// - Parameters:
///   - ray: The ray we cast from the origin point to our viewport
///   - found_info: Populates general info on our object such as material
///   and hit information
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Sphere::hit(const Ray& ray, ObjectInfo& found_info, double t_min, double t_max) {
    Vector3 oc = ray.origin - center;
    double a = ray.direction.length_squared();
    double half_b = dot(oc, ray.direction);
    double c = oc.length_squared() - radius * radius;
    
    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    double sqrtd = std::sqrt(discriminant);
    
    // Find the nearest t that lies in our range if we did hit
    // our sphere, start with - root
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        // - Root didnt work try + root
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }
    // Cache our info of this hit
    info.t = root;
    info.ray_of_hit = ray;
    info.point_of_hit = info.ray_of_hit.at(info.t);
    Vector3 outward_normal = (info.point_of_hit - center) / radius;
    set_surface_normal(info.ray_of_hit, outward_normal);
    set_uv(outward_normal, info.u, info.v);
    // Send our info back
    found_info = info;
    return true;
}

/// Maps the coordinates of a point on a sphere to a plane representing our texture
/// using u and v to mark our horizontal and vertical position respectively
/// - Parameters:
///   - point: The point on our sphere to be mapped
///   - u: The mapped horizontal position of our point on the texture
///   - v: The mapped vertical position of our point on the texture
void Sphere::set_uv(const Point3& point, double& u, double& v) {
    double theta = acos(-point.y);
    double phi = atan2(-point.z, point.x + pi);
    
    u = phi / (2 * pi);
    v = theta / pi;
}

/// A box around our sphere object used to speed up hit detection
AABB Sphere::make_bounding_box() const {
    return AABB(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
}
