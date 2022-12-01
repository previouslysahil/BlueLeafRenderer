//
//  Ray.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Ray.hpp"

/// Empty constructor makes 0,0,0 point with 0,0,0 direction
Ray::Ray(): origin(Point3()), direction(Vector3()) {}

/// Makes ray with start point and direction
/// - Parameters:
///   - origin: Start point A
///   - direction: Direction vector b
Ray::Ray(Point3 origin, Vector3 direction): origin(origin), direction(direction) {}

/// Calculates the t-th point along the direction specified by our vector
/// using math equation P(t) = A + bt
/// - Parameter t: The direction value along our ray
Point3 Ray::at(double t) const {
    return origin + direction * t;
}
