//
//  Vector3.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Vector3.hpp"

#include <cmath>
#include <iostream>

/// Default constructor makes zerod double vector
Vector3::Vector3(): x(0), y(0), z(0) {}


/// Makes double vector with given x, y, z coordinates
/// - Parameters:
///   - x: x coord
///   - y: y coord
///   - z: z coord
Vector3::Vector3(double x, double y, double z): x(x), y(y), z(z) {}

/// Length of vector using standard formula
double Vector3::length() const {
    return std::sqrt(length_squared());
}

/// Length of vector squared
double Vector3::length_squared() const {
    return x * x + y * y + z * z;
}

const double& Vector3::operator[](int i) const {
    if (i == 0) {
        return x;
    } else if (i == 1) {
        return y;
    } else if (i == 2) {
        return z;
    } else {
        std::cout << "Out of bound Vector access" << std::endl;
        exit(EXIT_FAILURE);
    }
}

/// Checks if our vector is basically zero
bool Vector3::near_zero() const {
    const auto s = 1e-8;
    return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
}
