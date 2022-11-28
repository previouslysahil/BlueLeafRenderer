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

inline double dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vector3 cross(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
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

inline Vector3 random_in_unit_disk() {
    while (true) {
        Vector3 p = Vector3(random_double(-1, 1), random_double(-1, 1), 0.0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline Vector3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline Vector3 reflect(const Vector3& v, const Vector3& n) {
    return v - n * 2 * dot(v, n);
}

inline Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
    double cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3 r_out_perp = (uv + n * cos_theta) * etai_over_etat;
    Vector3 r_out_parallel = n * -sqrt(fabs(1.0 - r_out_perp.length_squared()));
    return r_out_perp + r_out_parallel;
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

inline Color operator *(const Color& c1, const Color& c2) {
    return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}

inline Color operator /(const Color& c, const double t) {
    return Color(c.r / t, c.g / t, c.b / t);
}

inline Color sqrt_color(const Color& c) {
    return Color(std::sqrt(c.r), std::sqrt(c.g), std::sqrt(c.b));
}

inline double reflectance(double cosine, double ref_idx) {
    // Schlicks approx for reflectance
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

#endif /* LinearAlgebra_hpp */
