//
//  Renderer.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

#include "Renderer.hpp"

/// Makes our color at the point of intersection of our ray
/// - Parameter r: Ray cast from origin to direction
Color Renderer::ray_color(const Ray &r) const {
    Vector3 unit_direction = unit_vector(r.direction);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return Color(1.0f, 1.0f, 1.0f) * (1.0f - t) + Color(0.5f, 0.7f, 1.0f) * t;
}

/// This function will generate the image buffer storing the pixel values of our image
/// in one contiguous buffer in the format R G B R G B R G B . . .
/// - Parameters:
///   - width: the image width
///   - height: the image height
uint32_t* Renderer::render_buffer(int width, int height) const {
    // Setup 'world'
    // Image
    float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    float image_width = static_cast<float>(width);
    float image_height = static_cast<float>(height);
    // Camera
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0f;
    // Now we essentially make the vector plane of our viewport where we will
    // cast our ray
    Point3 origin = Point3(0.0f, 0.0f, 0.0f);
    Vector3 horizontal = Vector3(viewport_width, 0.0f, 0.0f);
    Vector3 vertical  = Vector3(0.0f, viewport_height, 0.0f);
    Point3 lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - Vector3(0.0f, 0.0f, focal_length);
    // Create our buffer with enough size for all our pixels
    uint32_t* buffer = new uint32_t[width * height];
    // Make an incrementer so we know when to move on to our next
    // set of pixel
    int pos = 0;
    // Generate image top down, left right so buffer stores pixel values
    // in their proper spot
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            // Generate values
            float u = float(i) / (image_width - 1.0f);
            float v = float(j) / (image_height - 1.0f);
            // Cast our ray
            Ray r(origin, lower_left_corner + horizontal * u + vertical * v - origin);
            // Make our colors
            Color color = ray_color(r);
            // Place in our buffer
            buffer[pos] = color.pixel();
            // Move to our next pixel set
            pos += 1;
        }
    }
    return buffer;
}
