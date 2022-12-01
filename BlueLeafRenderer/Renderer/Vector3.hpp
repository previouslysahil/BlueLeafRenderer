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
    double x;
    double y;
    double z;
public:
    Vector3();
    Vector3(double x, double y, double z);
    
    double length() const;
    double length_squared() const;
    bool near_zero() const;
    const double& operator[](int i) const;
};

/// The mathematical application and interchangeability of points
/// and vectors is so ubiqutous for our purposes they will use the
/// same underlying class
using Point3 = Vector3;

#endif /* Vector3_hpp */
