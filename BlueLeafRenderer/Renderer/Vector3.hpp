//
//  Vector3.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef Vector3_hpp
#define Vector3_hpp

class Vector3 {
public:
    float x;
    float y;
    float z;
public:
    Vector3();
    Vector3(float x, float y, float z);
    
    float length() const;
    float length_squared() const;
};

/// The mathematical application and interchangeability of points
/// and vectors is so ubiqutous for our purposes they will use the
/// same underlying class
using Point3 = Vector3;

#endif /* Vector3_hpp */
