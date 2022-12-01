//
//  BVH.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/29/22.
//

#include "BVH.hpp"

#include <vector>

BVH::BVH(std::vector<Object>& objects): objects(objects) {
    // Size of our objects
    int N = int(objects.size());
    // Construct our empty tree
    nodes = std::vector<BVHNode>(N * 2 - 1);
    int nodes_created = 0;
    BVHNode& root = nodes[nodes_created++];
    root.first_obj_idx = 0;
    root.objs_count = N;
    // Now we make the bounds of this node
    update_node_bounds(nodes, objects, 0);
    // Then we subdivide our root
    subdivide_node(nodes, objects, 0, nodes_created);
    
}

bool BVH::hit(const Ray& ray, ObjectInfo& info, int idx, double t_min, double t_max) {
    BVHNode& node = nodes[idx];
    // Check if we hit this nodes bounding box first
    if (!node.bounding_box.hit(ray, t_min, t_max)) {
        return false;
    }
    // If this is a leaf node check if we hit the objects it has
    // it should only have one object
    if (node.objs_count != 0) {
        // The only time we will have more than one object in this loop is if one side of a
        // partition failed to have even a single object, this is an edge case but may occur
        if (node.objs_count == 1) {
            return objects[node.first_obj_idx].hit(ray, info, t_min, t_max);
        } else {
            bool hit_object = false;
            double curr_max_t = t_max;
            // Loop through our objects and find the closest one we hit
            for (int i = 0; i < node.objs_count; i++) {
                Object& object = objects[node.first_obj_idx + i];
                // Will only populate info we have a hit in our
                // t range
                if (object.hit(ray, info, t_min, curr_max_t)) {
                    hit_object = true;
                    // Reset t range so we register hit on
                    // the closest object
                    curr_max_t = object.info.t;
                }
            }
            return hit_object;
        }
    } else {
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

void BVH::update_node_bounds(std::vector<BVHNode>& nodes, std::vector<Object>& objects, int idx) {
    BVHNode& node = nodes[idx];
    // Init our nodes box, this will always be overwritten by
    // the first object
    node.bounding_box = AABB(Point3(-1e30, -1e30, -1e30), Point3(1e30, 1e30, 1e30));
    for (int i = 0; i < node.objs_count; i++) {
        // Make a bounding box using the existing nodes bounding
        // box and this objects bounding box
        node.bounding_box = surrounding_box(node.bounding_box, objects[node.first_obj_idx + i].bounding_box);
    }
}

void BVH::subdivide_node(std::vector<BVHNode>& nodes, std::vector<Object>& objects, int idx, int& nodes_created) {
    // We terminate if we have only one object in our node (note for triangles
    // that make up a plane this won't work since they would have the same center
    // so would probaby have to randomly assign or something
    BVHNode& node = nodes[idx];
    // If we only have one object no need to recurse
    if (node.objs_count == 1) return;;
    // Find the axis we will split our bounding box on
    Vector3 length = node.bounding_box.maximum - node.bounding_box.minimum;
    int axis = longest_axis(length);
    // Get the middle point of the AABB on this axis
    double middle_point = node.bounding_box.minimum[axis] + length[axis] * 0.5;
    // Now we partition our objects around this middle point
    int i = node.first_obj_idx;
    int j = i + node.objs_count;
    // Go through list and if center is not on the left of or midpoint
    // swap with an element at the end of our list (moving it to not left
    // side and then decrementing so we never swap existing not left
    // elements to the left side)
    while (i <= j) {
        if (objects[i].center[axis] < middle_point) {
            i++;
        } else {
            std::swap(objects[i], objects[j--]);
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
    update_node_bounds(nodes, objects, left_idx);
    update_node_bounds(nodes, objects, right_idx);
    // Now subdivide the left and right children again
    subdivide_node(nodes, objects, left_idx, nodes_created);
    subdivide_node(nodes, objects, right_idx, nodes_created);
}
