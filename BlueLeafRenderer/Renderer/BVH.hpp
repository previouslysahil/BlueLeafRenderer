//
//  BVH.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/29/22.
//

#ifndef BVH_hpp
#define BVH_hpp

#include "Object.hpp"
#include "AABB.hpp"

#include <vector>

struct BVHNode {
public:
    int left;
    int right;
    int first_obj_idx;
    int objs_count;
    AABB bounding_box;
};

class BVH {
public:
    std::vector<BVHNode> nodes;
    std::vector<Object> objects;
    std::vector<int> objects_idx;
public:
    BVH();
    
    void add(Object& object);
    void clear();
    
    void generate_tree();
    void print_tree();
    
    bool hit(const Ray& ray, ObjectInfo& info, int idx, double t_min, double t_max);
private:
    void print_tree_helper(int idx);
    void create_node_bounding_box(int idx);
    void split_node(int idx, int& nodes_created);
};

#endif /* BVH_hpp */
