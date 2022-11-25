//
//  LinearAlgebra.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef LinearAlgebra_hpp
#define LinearAlgebra_hpp

#include "Vector3.hpp"
#include "Color.hpp"
#include "Utility.hpp"

#include <cmath>

inline Vector3 operator +(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vector3 operator -(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector3 operator -(const Vector3& v) {
    return Vector3(-v.x, -v.y, -v.z);
}

inline Vector3 operator *(const Vector3& v, const double t) {
    return Vector3(v.x * t, v.y * t, v.z * t);
}

inline Vector3 operator /(const Vector3& v, const double t) {
    return Vector3(v.x / t, v.y / t, v.z / t);
}

inline Vector3 unit_vector(const Vector3& v) {
    return v / v.length();
}

inline Vector3 random(double min, double max) {
    return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline Vector3 random_in_unit_sphere() {
    while (true) {
        Vector3 p = random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline Vector3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline double dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Color operator +(const Color& c1, const Color& c2) {
    return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}

inline Color operator +(const Color& c, const Vector3& v) {
    return Color(c.r + v.x, c.g + v.y, c.b + v.z);
}

inline Color operator *(const Color& c, const double t) {
    return Color(c.r * t, c.g * t, c.b * t);
}

inline Color operator /(const Color& c, const double t) {
    return Color(c.r / t, c.g / t, c.b / t);
}

inline Color sqrt_color(const Color& c) {
    return Color(std::sqrt(c.r), std::sqrt(c.g), std::sqrt(c.b));
}

#endif /* LinearAlgebra_hpp */
