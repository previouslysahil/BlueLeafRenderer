//
//  Utility.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef Utility_hpp
#define Utility_hpp

#include <cstdlib>

/// Generates a random double between 0 and 1
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
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

#endif /* Utility_hpp */
