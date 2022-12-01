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

#include <vector>

class Scene {
public:
    std::vector<Object> objects;
    
public:
    Scene();
    
    void add(Object& object);
    void clear();
    
    bool get_nearest_object(const Ray& ray, ObjectInfo& object_info, double t_min, double t_max);
};

#endif /* Scene_hpp */
