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
///   - object_info: Info that will contain important information
///   of the object and the point at which the ray hit our object, such as:
///   the type of our object, point of hit, surface normal at point of hit,
///   the ray that hit (which is the above ray), the object material, the t value
///   that our ray hit our point of hit, direction of surface normal
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Scene::get_nearest_object(const Ray& ray, ObjectInfo& object_info, double t_min, double t_max) {
    // No object found yet
    bool found_object = false;
    double curr_max_t = t_max;
    
    for (int i = 0; i < objects.size(); i++) {
        Object& object = objects[i];
        // Check if we hit this object in the range
        // of t values we are searching through
        // If we did hit we will run our hit caluclations
        // populating object info (even if we find a closer
        // object this will still always calculate our
        // object info which can slow down render times
        // unless we are using a bvh to do our searching)
        if (object.hit(ray, object_info, t_min, curr_max_t)) {
            found_object = true;
            // Move our search distance closer so
            // we could potentially find closer objects
            curr_max_t = object.info.t;
        }
    }
    return found_object;
}
