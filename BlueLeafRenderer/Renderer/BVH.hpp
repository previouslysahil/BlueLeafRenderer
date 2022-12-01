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

#include <memory>
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
    std::vector<Object>& objects;
public:
    BVH();
    BVH(std::vector<Object>& objects);
    
    bool hit(const Ray& ray, ObjectInfo& info, int idx, double t_min, double t_max);
private:
    void update_node_bounds(std::vector<BVHNode>& nodes, std::vector<Object>& objects, int idx);
    void subdivide_node(std::vector<BVHNode>& nodes, std::vector<Object>& objects, int idx, int& nodes_created);
};

#endif /* BVH_hpp */
