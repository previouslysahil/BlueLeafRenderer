//
//  Object.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/26/22.
//

#include "Object.hpp"

/// Initalizes all values to zeros or false
Object::Object(): info { Empty, Point3(), Vector3(), Ray(), Material(), 0, false }, center(), radius(0) {}

/// Initalizes all values to zeros or false except for center and radius which
/// define the circles location and sets the object type to sphere
/// USED ONLY FOR SPHERES
/// - Parameters:
///   - center: The center of our circle
///   - radius: The radius of our circle
///   - material: The material of this sphere object
Object::Object(Point3 center, double radius, Material& material): info { Sphere, Point3(), Vector3(), Ray(), Material(material), 0, false }, center(center), radius(radius) {
    switch (info.type) {
        case Sphere:
            bounding_box = sphere_bounding_box();
            return;
        case Empty:
            return;
        default:
            return;
    }
    return;
}

/// Determines which hit function we should use depending on the object type
/// - Parameters:
///   - ray: The ray we cast from the origin point to our viewport
///   - found_info: General info on our object such as material
///   and hit information
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Object::hit(const Ray& ray, ObjectInfo& found_info, double t_min, double t_max) {
    switch (info.type) {
        case Sphere:
            return sphere_hit(ray, found_info, t_min, t_max);
        case Empty:
            return false;
        default:
            return false;
    }
    return false;
}

/// Calculates the hit equation of sphere given by (A + tb - C)(A + tb - C) = r^2
/// by solving for t, less than 0 means no miss, exactly 0 means 1 hit, greater
/// than zero means two solutions, after getting this t value we can calculate
/// our point of hit and surface normal, returns true if we found a t and will
/// pass back our object info for use elsewhere
/// USED ONLY FOR SPHERES
/// - Parameters:
///   - ray: The ray we cast from the origin point to our viewport
///   - found_info: Populates general info on our object such as material
///   and hit information
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Object::sphere_hit(const Ray& ray, ObjectInfo& found_info, double t_min, double t_max) {
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
    // Send our info back
    found_info = info;
    return true;
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
void Object::set_surface_normal(const Ray& ray, const Vector3& outward_normal) {
    // Dot product less than 0 implies the ray and outward normal are goin
    // against each other AKA ray is outside object
    info.front_face = dot(ray.direction, outward_normal) < 0;
    // Based on the direction of our ray to our object/ if we are inside
    // or outside our object we change the surface normal
    info.surface_normal = info.front_face ? outward_normal : -outward_normal;
}

/// A box around our sphere object used to speed up hit detection
AABB Object::sphere_bounding_box() const {
    return AABB(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
}
