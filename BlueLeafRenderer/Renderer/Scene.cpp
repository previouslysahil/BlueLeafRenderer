//
//  Scene.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Scene.hpp"

/// Initializes object to empty
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
/// stored in our scene, if we have hit an object we then caclulate the point of hit
/// and surface normal
/// Returns true if we found and object to hit and passes back the point of hit,
/// surface normal and material of the object we hit
/// - Parameters:
///   - ray: The ray coming from the origin to our viewport
///   - point_of_hit: The point our ray hit our nearest object at
///   - surface_normal: The normal at the point our ray hit our object
///   - object_material: The material of the object we hit
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Scene::findNearestObject(const Ray& ray, Point3& point_of_hit, Vector3& surface_normal, Material& object_material, double t_min, double t_max) {
    // No object found yet
    bool found_object = false;
    int found_idx = -1;
    double curr_max_t = t_max;
    
    for (int i = 0; i < objects.size(); i++) {
        Object& object = objects[i];
        // Check if we hit this object in the range
        // of t values we are searching through
        if (object.hit(ray, t_min, curr_max_t)) {
            found_object = true;
            // Move our search distance closer so
            // we could potentially find closer objects
            curr_max_t = object.t;
            // Update our found index to use for hit calc
            found_idx = i;
        }
    }
    // Run our hit calculation if we found a nearest object
    // we do this hear to avoid unecessary hit calculation
    // above
    if (found_idx != -1) {
        Object& foundObject = objects[found_idx];
        foundObject.calculate_hit();
        // Pass back the point our object was hit at
        // the normal of the object at that point and
        // the material of the object
        point_of_hit = foundObject.point_of_hit;
        surface_normal = foundObject.surface_normal;
        object_material = foundObject.object_material;
    }
    return found_object;
}
