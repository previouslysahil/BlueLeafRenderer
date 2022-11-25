//
//  Vector3.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Vector3.hpp"

#include <cmath>

/// Default constructor makes zerod float vector
Vector3::Vector3(): x(0), y(0), z(0) {}


/// Makes float vector with given x, y, z coordinates
/// - Parameters:
///   - x: x coord
///   - y: y coord
///   - z: z coord
Vector3::Vector3(float x, float y, float z): x(x), y(y), z(z) {}

/// Length of vector using standard formula
float Vector3::length() const {
    return std::sqrt(length_squared());
}

/// Length of vector squared
float Vector3::length_squared() const {
    return x * x + y * y + z * z;
}
