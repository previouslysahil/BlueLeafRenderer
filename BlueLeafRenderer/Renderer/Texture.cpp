//
//  Texture.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/23/22.
//

#include "Texture.hpp"
#include "LinearAlgebra.hpp"

#include <cmath>

Texture::Texture() {}

SolidColor::SolidColor(): color_value() {}

SolidColor::SolidColor(Color color_value): color_value(color_value) {}

SolidColor::SolidColor(double red, double green, double blue): color_value(red, green, blue) {}

/// SolidColor will always simply return the initialized color value
/// - Parameters:
///   - u: The horizontal position of our object mapped to texture
///   - v: The vertical position of our object mapped to texture
///   - point_of_hit: The point our object was hit at
Color SolidColor::value(double u, double v, const Point3& point_of_hit) const {
    return color_value;
}

CheckerTexture::CheckerTexture(): odd(nullptr), even(nullptr) {}

CheckerTexture::CheckerTexture(SolidColor* odd, SolidColor* even, double scale): odd(odd), even(even), scale(scale) {}

/// CheckerTexture will alternate between our odd or even color to generate
/// our grid texture based on the multiplied sine values being less than 0 or not
/// - Parameters:
///   - u: The horizontal position of our object mapped to texture
///   - v: The vertical position of our object mapped to texture
///   - point_of_hit: The point our object was hit at
Color CheckerTexture::value(double u, double v, const Point3& point_of_hit) const {
    double sines = sin(scale * point_of_hit.x) * sin(scale * point_of_hit.y) * sin(scale * point_of_hit.z);
    
    if (sines < 0) {
        return odd->value(u, v, point_of_hit);
    } else {
        return even->value(u, v, point_of_hit);
    }
}

NoiseTexture::NoiseTexture(): perlin(Perlin()), scale(1) {}

NoiseTexture::NoiseTexture(double scale): perlin(Perlin()), scale(scale) {}

/// NoiseTexture will use perlin noise to generate our color value
/// at the point of hit coordinate
/// - Parameters:
///   - u: The horizontal position of our object mapped to texture
///   - v: The vertical position of our object mapped to texture
///   - point_of_hit: The point our object was hit at
Color NoiseTexture::value(double u, double v, const Point3& point_of_hit) const {
    return Color(1, 1, 1) * 0.5 * (perlin.noise(point_of_hit * scale) + 1.0);
}
