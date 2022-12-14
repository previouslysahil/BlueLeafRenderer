//
//  Scene.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Scene.hpp"

/// Initializes object and bvhs to empty
Scene::Scene(): objects(), bvhs() {};

/// Deallocates all our heap allocated objects in our scene
void Scene::deallocate_objects() {
    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
}

/// Deallocates all our heap allocated objects in our bvhs
void Scene::deallocate_bvhs() {
    for (int i = 0; i < bvhs.size(); i++) {
        bvhs[i].deallocate_objects();
    }
}

/// Deallocates all our heap allocated materials in our scene
void Scene::deallocate_materials() {
    for (int i = 0; i < materials.size(); i++) {
        delete materials[i];
    }
}

/// Deallocates all our heap allocated textures in our scene
void Scene::deallocate_textures() {
    for (int i = 0; i < textures.size(); i++) {
        delete textures[i];
    }
}

/// Adds a object to our scene
/// - Parameter object: The object to be added
void Scene::add(Object* object) {
    objects.push_back(object);
}

/// Adds a bvh to our scene and generate it's tree
/// - Parameter bvh: The bvh to be added
void Scene::add(BVH& bvh) {
    bvh.generate_tree();
    bvhs.push_back(bvh);
}

/// Adds a material to our scene
/// - Parameter material: The object to be added
void Scene::add(Material* material) {
    materials.push_back(material);
}

/// Adds a textures to our scene
/// - Parameter texture: The object to be added
void Scene::add(Texture* texture) {
    textures.push_back(texture);
}

/// Uses our ray and min and max ts to determine whether we have hit an object
/// stored in our scene, if we have hit an object we then caclulate the point of hit
/// and surface normal, this method will first check our bvhs for any hits and then
/// look through the remaining scene objects
/// Returns true if we found and object to hit and passes back the point of hit,
/// surface normal and material of the object we hit
/// - Parameters:
///   - ray: The ray coming from the origin to our viewport
///   - info: Info that will contain important information
///   of the object and the point at which the ray hit our object, such as:
///   the type of our object, point of hit, surface normal at point of hit,
///   the ray that hit (which is the above ray), the object material, the t value
///   that our ray hit our point of hit, direction of surface normal
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool Scene::get_nearest_object(const Ray& ray, ObjectInfo& info, double t_min, double t_max) {
    // No object found yet
    bool found_object = false;
    double curr_max_t = t_max;
    
    // Check our bvhs for a hit first since they don't run
    // hit calculations and check bounding boxs for a hit first
    for (int i = 0; i < bvhs.size(); i++) {
        BVH& bvh = bvhs[i];
        // Checks if we hit our bvh, and then recursively finds
        // the right object to hit searching through the tree
        if (bvh.hit(ray, info, 0, t_min, curr_max_t)) {
            found_object = true;
            // Move our search distance closer so
            // we could potentially find closer objects
            curr_max_t = info.t;
        }
    }
    // Exhausted bvhs so see if we can find a hit or a closer
    // hit with another scene object
    for (int i = 0; i < objects.size(); i++) {
        Object*& object = objects[i];
        // Check if we hit this object in the range
        // of t values we are searching through
        // If we did hit we will run our hit caluclations
        // populating object info (even if we find a closer
        // object this will still always calculate our
        // object info which can slow down render times
        // unless we are using a bvh to do our searching)
        if (object->hit(ray, info, t_min, curr_max_t)) {
            found_object = true;
            // Move our search distance closer so
            // we could potentially find closer objects
            curr_max_t = info.t;
        }
    }
    return found_object;
}
