//
//  Scene.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef Scene_hpp
#define Scene_hpp

#include "Object.hpp"
#include "Material.hpp"
#include "BVH.hpp"

#include <vector>

class Scene {
private:
    std::vector<Object> objects;
    std::vector<BVH> bvhs;
    
public:
    Scene();
    
    void add(Object& object);
    void add(BVH& bvh);
    void clear();
    
    bool get_nearest_object(const Ray& ray, ObjectInfo& info, double t_min, double t_max);
};

#endif /* Scene_hpp */
