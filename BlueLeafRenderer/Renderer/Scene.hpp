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
#include <memory>

class Scene {
private:
    std::vector<Object*> objects;
    std::vector<BVH> bvhs;
    std::vector<Material*> materials;
public:
    Scene();
    void deallocate_objects();
    void deallocate_bvhs();
    void deallocate_materials();
    
    void add(Object* object);
    void add(BVH& bvh);
    void add(Material* material);
    
    bool get_nearest_object(const Ray& ray, ObjectInfo& info, double t_min, double t_max);
};

#endif /* Scene_hpp */
