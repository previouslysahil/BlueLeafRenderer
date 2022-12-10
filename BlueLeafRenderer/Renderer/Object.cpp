//
//  Object.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/26/22.
//

#include "Object.hpp"

/// Initalizes all values to zeros or false
Object::Object(): info { Point3(), Vector3(), Ray(), nullptr, 0, false }, bounding_box() {}

Object::Object(Material* material, Point3 center): info { Point3(), Vector3(), Ray(), material, 0, false }, bounding_box(), center(center) {}

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
