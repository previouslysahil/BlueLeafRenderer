//
//  BVH.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/29/22.
//  Based on: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/

#include "BVH.hpp"

#include <iostream>

/// Empty initializes all arrays
BVH::BVH(): nodes(), objects(), objects_idx() {}

/// Deallocates all our heap allocated objects in our bvh tree
void BVH::deallocate_objects() {
    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
}

/// Adds a object to our scene
/// - Parameter object: The object to be added
void BVH::add(Object* object) {
    objects.push_back(object);
}

/// Removes all objects from our scene
void BVH::clear() {
    objects.clear();
}

/// Creates our BVH tree using objects
void BVH::generate_tree() {
    // Size of our objects
    int N = int(objects.size());
    // Construct our empty tree (may not
    // get completely filled if edge cases hit)
    nodes = std::vector<BVHNode>(N * 2 - 1);
    // Track our objects indicies so we do not have to physically
    // swap our objects in our objects array when partitioning
    objects_idx = std::vector<int>(objects.size());
    for (int i = 0; i < objects_idx.size(); i++) {
        objects_idx[i] = i;
    }
    // Make our root node
    int nodes_created = 0;
    // Inc nodes created after we make root
    BVHNode& root = nodes[nodes_created++];
    root.first_obj_idx = 0;
    root.objs_count = N;
    // Now we make the bounds of this node
    create_node_bounding_box(0);
    // Then we subdivide our root
    split_node(0, nodes_created);
}

/// Generates a preorder traversal of our tree
void BVH::print_tree() {
    std::cout << "Printed tree: ";
    print_tree_helper(0);
    std::cout << std::endl;
}

/// Recursive call of traversal
/// - Parameter idx: The index of our current node
void BVH::print_tree_helper(int idx) {
    BVHNode& node = nodes[idx];
    // Terminate if leaf
    if (node.objs_count != 0) {
        std::cout << "(";
        for (int i = 0; i < node.objs_count; i++) {
            // Get our object through out objects index array since node tracs objects_idx
            Object* object = objects[objects_idx[node.first_obj_idx + i]];
            std::cout << object->info.material->albedo.g;
            if (i != node.objs_count - 1) {
                std::cout << " ";
            }
        }
        std::cout << ")";
    } else {
        std::cout << "I ";
        print_tree_helper(node.left);
        std::cout << "|term left|";
        print_tree_helper(node.right);
    }
}

/// Uses our ray and min and max ts to determine whether we have hit an object
/// stored in our scene, if we have hit an object we then caclulate the point of hit
/// and surface normal, the method recurses through our bvh starting at
/// the subtree defined by our idx and checking bounding boxes for hits in order
/// to minimize hit calculations.
/// Returns true if we found and object to hit and passes back the point of hit,
/// surface normal and material of the object we hit
/// UNROLL THIS TO WHILE LOOP AT SOME POINT
/// - Parameters:
///   - ray: The ray coming from the origin to our viewport
///   - info: Info that will contain important information
///   of the object and the point at which the ray hit our object, such as:
///   the type of our object, point of hit, surface normal at point of hit,
///   the ray that hit (which is the above ray), the object material, the t value
///   that our ray hit our point of hit, direction of surface normal
///   - idx: The index of our current node
///   - t_min: The minimum t value we should acknowledge as a hit
///   - t_max: The maximum t value we should acknowledge as a hit
bool BVH::hit(const Ray& ray, ObjectInfo& info, int idx, double t_min, double t_max) {
    BVHNode& node = nodes[idx];
    // Check if we hit this nodes bounding box first
    if (!node.bounding_box.hit(ray, t_min, t_max)) {
        return false;
    }
    // If this is a leaf node it will have a non zero object count
    if (node.objs_count != 0) {
        // The only time we will have more than one object in this loop is if one side of a
        // partition failed to have even a single object, this is an edge case but may occur
        // i.e. circle inside circle with same center
        if (node.objs_count == 1) {
            // Get our object through out objects index array since node tracs objects_idx
            return objects[objects_idx[node.first_obj_idx]]->hit(ray, info, t_min, t_max);
        } else {
            bool hit_object = false;
            double curr_max_t = t_max;
            // Loop through our objects and find the closest one we hit
            for (int i = 0; i < node.objs_count; i++) {
                // Get our object through out objects index array since node tracs objects_idx
                Object* object = objects[objects_idx[node.first_obj_idx + i]];
                // Will only populate info we have a hit in our
                // t range
                if (object->hit(ray, info, t_min, curr_max_t)) {
                    hit_object = true;
                    // Reset t range so we register hit on
                    // the closest object
                    curr_max_t = object->info.t;
                }
            }
            return hit_object;
        }
    } else {
        // Internal node so check children for hits
        // Check if we hit something on the left side of this node
        bool hit_left = hit(ray, info, node.left, t_min, t_max);
        // If we did then our info will be populated with a new t value
        // so bound hits on the right tree recursion in this new t range
        // so we always get the closest object
        bool hit_right = hit(ray, info, node.right, t_min, hit_left ? info.t : t_max);
        // If we hit something return true since info will be populated
        // with proper hit information
        return hit_left || hit_right;
    }
}

/// Generates a bounding box around all the objects that are contained
/// in this node
/// - Parameter idx: The index of our current node
void BVH::create_node_bounding_box(int idx) {
    BVHNode& node = nodes[idx];
    // Init our nodes box, this will always be overwritten by
    // the first object
    node.bounding_box = AABB(Point3(1e30, 1e30, 1e30), Point3(-1e30, -1e30, -1e30));
    for (int i = 0; i < node.objs_count; i++) {
        // Make a bounding box using the existing nodes bounding
        // box and this objects bounding box
        // Get our object through out objects index array since node tracks objects_idx
        node.bounding_box = surrounding_box(node.bounding_box, objects[objects_idx[node.first_obj_idx + i]]->bounding_box);
    }
}

/// Splits a node by finding the middle point of the longest axis of this node's
/// bounding box and then paritioning the objects this node owns to the left
/// or non left (right + center) of the middle point, after performing the swaps
/// necessary for this partitioning the node will populate its left child with the
/// left parition and right child with the non left parition, generating a bounding
/// box for each of these children and then recursing through them until we have
/// only one object owned by a node or we can no longer partition a nodes
/// governed objects into two sets
/// - Parameters:
///   - idx: The index of our current node
///   - nodes_created: The number of nodes created and populated so far
///   used to organize nodes in our array representation of our bvh tree
void BVH::split_node(int idx, int& nodes_created) {
    // We terminate if we have only one object in our node (note for triangles
    // that make up a plane this won't work since they would have the same center
    // so would probaby have to randomly assign or something
    BVHNode& node = nodes[idx];
    // If we only have one object no need to recurse
    if (node.objs_count == 1) {
        return;
    }
    // Find the axis we will split our bounding box on
    Vector3 length = node.bounding_box.maximum - node.bounding_box.minimum;
    int axis = longest_axis(length);
    // Get the middle point of the AABB on this axis
    double middle_point = node.bounding_box.minimum[axis] + length[axis] * 0.5;
    // Now we partition our objects around this middle point
    int i = node.first_obj_idx;
    int j = i + node.objs_count - 1;
    // Go through list and if center is not on the left of or midpoint
    // swap with an element at the end of our list (moving it to not left
    // side and then decrementing so we never swap existing not left
    // elements to the left side)
    while (i <= j) {
        // Get our object through out objects index array since node tracs objects_idx
        if (objects[objects_idx[i]]->center[axis] < middle_point) {
            i++;
        } else {
            // IMPORTANT: Swap our object through out objects index array since
            // this saves resources (no need to swap entire object)
            std::swap(objects_idx[i], objects_idx[j--]);
        }
    }
    // I will always stop at the beginning of our not left side
    // objects so subtract to get left side count
    int left_side_count = i - node.first_obj_idx;
    // If despite this middle point partition one of the sides
    // is empty we just keep this node and end our recursion here
    if (left_side_count == 0 || left_side_count == node.objs_count) {
        return;
    }
    // In our array representing our BVH we simply
    // increment to get our child node's positions
    int left_idx = nodes_created++;
    int right_idx = nodes_created++;
    // Store the idx of our child nodes so our current
    // node can access them
    node.left = left_idx;
    node.right = right_idx;
    // Create our left child's 'objects'
    nodes[left_idx].first_obj_idx = node.first_obj_idx;
    nodes[left_idx].objs_count = left_side_count;
    // Create our second child's 'objects
    nodes[right_idx].first_obj_idx = i;
    nodes[right_idx].objs_count = node.objs_count - left_side_count;
    // We now 'erase' this current nodes object counts
    // since we have split them to the right and left childs
    node.objs_count = 0;
    // Now make sure to update our new children node's bounds
    create_node_bounding_box(left_idx);
    create_node_bounding_box(right_idx);
    // Now break the left and right children again
    split_node(left_idx, nodes_created);
    split_node(right_idx, nodes_created);
}
