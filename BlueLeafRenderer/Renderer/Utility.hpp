//
//  Utility.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef Utility_hpp
#define Utility_hpp

#include <cstdlib>
#include <random>

const double pi = 3.1415926535897932385;

/// Generates a random double between 0 and 1
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

/// Generates a random double between a min and max
/// - Parameters:
///   - min: Min double val
///   - max: Max double val
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

/// Clamps a value between a min and max
/// - Parameters:
///   - x: Value to be clamped
///   - min: Min clamp
///   - max: Max clamp
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

/// Converts a degree to a radian
/// - Parameter degrees: The angles degree
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#endif /* Utility_hpp */
