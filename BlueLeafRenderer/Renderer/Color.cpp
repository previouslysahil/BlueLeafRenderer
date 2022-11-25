//
//  Color.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Color.hpp"
#include "Utility.hpp"

/// Default constructor makes a solid black color
Color::Color(): r(0), b(0), g(0), a(0) {}


/// Creates double RGBA values within the range of 0-1, alpha preset to 1.0f
/// to 255
/// - Parameters:
///   - r: Red value
///   - g: Green value
///   - b: Blue value
Color::Color(double r, double g, double b): r(r), b(b), g(g), a(0) {}


/// Creates double RGBA values within the range of 0-1
/// - Parameters:
///   - r: Red value
///   - g: Green value
///   - b: Blue value
///   - a: Alpha value
Color::Color(double r, double g, double b, double a): r(r), b(b), g(g), a(a) {}


/// Uses casting bit shifting to form a 32 bit color pixel from doubles RGBA values
uint32_t Color::rgba() const {
    // Convert our colors double values to RGBA space
    uint8_t r8 = static_cast<uint8_t>(256.0 * clamp(r, 0.0, 0.999));
    uint8_t g8 = static_cast<uint8_t>(256.0 * clamp(g, 0.0, 0.999));
    uint8_t b8 = static_cast<uint8_t>(256.0 * clamp(b, 0.0, 0.999));
    uint8_t a8 = static_cast<uint8_t>(255.0);
    // Bit shift to store each pixel in a 32 bit uint
    return r8 << 24 | g8 << 16 | b8 << 8 | a8;
}
