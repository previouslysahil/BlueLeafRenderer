//
//  Triangle.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/26/22.
//

#include "Triangle.hpp"
#include "LinearAlgebra.hpp"

#include <iostream>

Triangle::Triangle(): Object(), vertex0(), vertex1(), vertex2() {}

Triangle::Triangle(Point3 vertex0, Point3 vertex1, Point3 vertex2, Material* material): Object(material, (vertex0 + vertex1 + vertex2) * (1.0 / 3.0)), vertex0(vertex0), vertex1(vertex1), vertex2(vertex2) {}

bool Triangle::hit(const Ray& ray, ObjectInfo& found_info, double t_min, double t_max) {
    const double eps = 0.0000001;
    Vector3 edge1, edge2, h, s, q;
    double a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = cross(ray.direction, edge2);
    a = dot(edge1, h);
    if (a > -eps && a < eps) {
        return false; // Ray parallel to triangle
    }
    f = 1.0 / a;
    s = ray.origin - vertex0;
    u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    q = cross(s, edge1);
    v = f * dot(ray.direction, q);
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }
    // Calculate our t
    double root = f * dot(edge2, q);
    // Check if we are in our bounds
    if (root < t_min || t_max < root) {
        return false;
    } else {
        info.t = root;
        info.ray_of_hit = ray;
        info.point_of_hit = info.ray_of_hit.at(info.t);
        Vector3 outward_normal = cross(edge1, edge2);
        set_surface_normal(info.ray_of_hit, outward_normal);
        // Send our info back
        found_info = info;
        return true;
    }
}

AABB Triangle::make_bounding_box() const {
    AABB box(Point3(1e30, 1e30, 1e30), Point3(-1e30, -1e30, -1e30));
    
    box.minimum.x = fmin(box.minimum.x, vertex0.x);
    box.minimum.y = fmin(box.minimum.y, vertex0.y);
    box.minimum.z = fmin(box.minimum.z, vertex0.z);
    box.minimum.x = fmin(box.minimum.x, vertex1.x);
    box.minimum.y = fmin(box.minimum.y, vertex1.y);
    box.minimum.z = fmin(box.minimum.z, vertex1.z);
    box.minimum.x = fmin(box.minimum.x, vertex2.x);
    box.minimum.y = fmin(box.minimum.y, vertex2.y);
    box.minimum.z = fmin(box.minimum.z, vertex2.z);
    
    box.maximum.x = fmax(box.maximum.x, vertex0.x);
    box.maximum.y = fmax(box.maximum.y, vertex0.y);
    box.maximum.z = fmax(box.maximum.z, vertex0.z);
    box.maximum.x = fmax(box.maximum.x, vertex1.x);
    box.maximum.y = fmax(box.maximum.y, vertex1.y);
    box.maximum.z = fmax(box.maximum.z, vertex1.z);
    box.maximum.x = fmax(box.maximum.x, vertex2.x);
    box.maximum.y = fmax(box.maximum.y, vertex2.y);
    box.maximum.z = fmax(box.maximum.z, vertex2.z);
    
    return box;
}
