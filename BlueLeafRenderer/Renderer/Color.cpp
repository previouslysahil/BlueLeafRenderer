//
//  Color.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Color.hpp"
#include <iostream>

/// Default constructor makes a solid black color
Color::Color(): r(0), b(0), g(0), a(0) {}


/// Creates float RGBA values within the range of 0-1, alpha preset to 1.0f
/// to 255
/// - Parameters:
///   - r: Red value
///   - g: Green value
///   - b: Blue value
Color::Color(float r, float g, float b): r(r), b(b), g(g), a(0) {}


/// Creates float RGBA values within the range of 0-1
/// - Parameters:
///   - r: Red value
///   - g: Green value
///   - b: Blue value
///   - a: Alpha value
Color::Color(float r, float g, float b, float a): r(r), b(b), g(g), a(a) {}


/// Uses casting bit shifting to form a 32 bit color pixel from floats RGBA values
uint32_t Color::pixel() const {
    // Convert our colors float values to RGBA space
    uint8_t r8 = static_cast<uint8_t>(255.999f * r);
    uint8_t g8 = static_cast<uint8_t>(255.999f * g);
    uint8_t b8 = static_cast<uint8_t>(255.999f * b);
    uint8_t a8 = static_cast<uint8_t>(255.0f);
    // Bit shift to store each pixel in a 32 bit uint
    return r8 << 24 | g8 << 16 | b8 << 8 | a8;
}
