//
//  World.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "World.hpp"

World::World(): spheres() {};

/// Adds a sphere to our world
/// - Parameter sphere: The sphere to be added
void World::add(Sphere& sphere) {
    spheres.push_back(sphere);
}

/// Removes all spheres from our world
void World::clear() {
    spheres.clear();
}

/// Uses our ray and min and max ts to determine whether we have hit an object
/// stored in our world
/// - Parameters:
///   - ray: The ray coming from the origin to our viewport
///   - point_of_hit: The point our ray hit our nearest object at
///   - surface_normal: The normal at the point our ray hit our object
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool World::findNearestObject(const Ray& ray, Point3& point_of_hit, Vector3& surface_normal, double t_min, double t_max) {
    bool found_object = false;
    double found_t = t_max;
    int found_idx = -1;
    
    for (int i = 0; i < spheres.size(); i++) {
        Sphere& sphere = spheres[i];
        if (sphere.hit(ray, t_min, found_t)) {
            found_object = true;
            found_t = sphere.t;
            found_idx = i;
        }
    }
    // Run our hit calculation if we found a nearest object
    if (found_idx != -1) {
        Sphere& foundSphere = spheres[found_idx];
        foundSphere.calculate_hit();
        point_of_hit = foundSphere.point_of_hit;
        surface_normal = foundSphere.surface_normal;
    }
    return found_object;
}
