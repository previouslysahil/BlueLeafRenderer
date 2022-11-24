//
//  Renderer.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

#include "Renderer.hpp"

/// This function will generate the image buffer storing the pixel values of our image
/// in one contiguous buffer in the format R G B R G B R G B . . .
/// - Parameters:
///   - width: the image width
///   - height: the image height
uint32_t* Renderer::render_buffer(int width, int height) {
    // Create our buffer with enough size for all our pixels
    uint32_t* buffer = new uint32_t[width * height];
    // Make an incrementer so we know when to move on to our next
    // set of pixels
    int pos = 0;
    // Generate image
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            // Generate values
            auto r = double(i) / (256 - 1);
            auto g = double(j) / (256 - 1);
            auto b = 0.25;
            // Cast to int
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
            // Bit shift to get into 32 bit format
            uint32_t pixel = static_cast<uint8_t>(ir) << 24 | static_cast<uint8_t>(ig) << 16 | static_cast<uint8_t>(ib) << 8 | 255;
            // Place in our buffer
            buffer[pos] = pixel;
            // Move to our next pixel set
            pos += 1;
        }
    }
    return buffer;
}
