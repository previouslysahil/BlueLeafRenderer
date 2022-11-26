//
//  Scene.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Scene.hpp"

Scene::Scene(): objects() {};

/// Adds a object to our scene
/// - Parameter object: The object to be added
void Scene::add(Object& object) {
    objects.push_back(object);
}

/// Removes all objects from our scene
void Scene::clear() {
    objects.clear();
}

/// Uses our ray and min and max ts to determine whether we have hit an object
/// stored in our scene
/// - Parameters:
///   - ray: The ray coming from the origin to our viewport
///   - point_of_hit: The point our ray hit our nearest object at
///   - surface_normal: The normal at the point our ray hit our object
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Scene::findNearestObject(const Ray& ray, Point3& point_of_hit, Vector3& surface_normal, double t_min, double t_max) {
    bool found_object = false;
    double found_t = t_max;
    int found_idx = -1;
    
    for (int i = 0; i < objects.size(); i++) {
        Object& object = objects[i];
        if (object.hit(ray, t_min, found_t)) {
            found_object = true;
            found_t = object.t;
            found_idx = i;
        }
    }
    // Run our hit calculation if we found a nearest object
    if (found_idx != -1) {
        Object& foundObject = objects[found_idx];
        foundObject.calculate_hit();
        point_of_hit = foundObject.point_of_hit;
        surface_normal = foundObject.surface_normal;
    }
    return found_object;
}
