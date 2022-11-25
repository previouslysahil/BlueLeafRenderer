//
//  Ray.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "Vector3.hpp"
#include "LinearAlgebra.hpp"

class Ray {
public:
    Point3 origin;
    Vector3 direction;
    
public:
    Ray();
    Ray(Point3 origin, Vector3 direction);
    
    Point3 at(double t) const;
};

#endif /* Ray_hpp */
