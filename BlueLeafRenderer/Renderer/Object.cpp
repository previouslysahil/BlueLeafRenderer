//
//  Object.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/26/22.
//

#include "Object.hpp"

/// Initalizes all values to zeros or false
Object::Object(): type(Empty), point_of_hit(), surface_normal(), object_material(), ray_of_hit(), t(0), front_face(false), center(), radius(0) {}

/// Initalizes all values to zeros or false except for center and radius which
/// define the circles location and sets the object type to sphere
/// USED ONLY FOR SPHERES
/// - Parameters:
///   - center: The center of our circle
///   - radius: The radius of our circle
///   - object_material: The material of this sphere object
Object::Object(Point3 center, double radius, Material& object_material): type(Sphere), point_of_hit(), surface_normal(), object_material(object_material), ray_of_hit(), t(0), front_face(false), center(center), radius(radius) {}

/// Determines which hit function we should use depending on the object type
/// - Parameters:
///   - ray: The ray we cast from the origin point to our viewport
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Object::hit(const Ray& ray, double t_min, double t_max) {
    switch (type) {
        case Sphere:
            return sphere_hit(ray, t_min, t_max);
        case Empty:
            return false;
        default:
            return false;
    }
    return false;
}

/// Calculates the hit equation of sphere given by (A + tb - C)(A + tb - C) = r^2
/// by solving for t, less than 0 means no miss, exactly 0 means 1 hit, greater
/// than zero means two solutions, returns the t value along our ray that we
/// hit our sphere or -1.0 if we did not hit our sphere
/// USED ONLY FOR SPHERES
/// - Parameters:
///   - ray: The ray we cast from the origin point to our viewport
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Object::sphere_hit(const Ray& ray, double t_min, double t_max) {
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
    // Cache our t and ray of hit in case we want to calculate
    // this hit
    t = root;
    ray_of_hit = ray;
    return true;
}


/// Determines which calculate hit function we should use depending on the object type
void Object::calculate_hit() {
    switch (type) {
        case Sphere:
            sphere_calculate_hit();
            return;
        case Empty:
            return;
        default:
            return;
    }
    return;
}

/// This function assumes that t has been set which will always occur if the hit function
/// returns true, assuming yes it will generate the point our ray hit our object at and also
/// generate the surface normal
/// USED ONLY FOR SPHERES
void Object::sphere_calculate_hit() {
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
void Object::set_surface_normal(const Ray& ray, const Vector3& outward_normal) {
    // Dot product less than 0 implies the ray and outward normal are goin
    // against each other AKA ray is outside object
    front_face = dot(ray.direction, outward_normal) < 0;
    // Based on the direction of our ray to our object/ if we are inside
    // or outside our object we change the surface normal
    surface_normal = front_face ? outward_normal : -outward_normal;
}
