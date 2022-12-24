//
//  Texture.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/23/22.
//

#include "Texture.hpp"

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
