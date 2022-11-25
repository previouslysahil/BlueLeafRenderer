//
//  World.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef World_hpp
#define World_hpp

#include "Sphere.hpp"

#include <vector>

class World {
public:
    std::vector<Sphere> spheres;
    
public:
    World();
    
    void add(Sphere& sphere);
    void clear();
    
    bool findNearestObject(const Ray& ray, Point3& point_of_hit, Vector3& surface_normal, double t_min, double t_max);
};

#endif /* World_hpp */
