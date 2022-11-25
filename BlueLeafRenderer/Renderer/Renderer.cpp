//
//  Renderer.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

#include "Renderer.hpp"
#include "Sphere.hpp"

#include <cmath>
#include <limits>

/// Makes our color at the point of intersection of our ray
/// - Parameter r: Ray cast from origin to direction
Color Renderer::ray_color(const Ray& ray, World& world) const {
    Point3 point_of_hit;
    Vector3 surface_normal;
    if (world.findNearestObject(ray, point_of_hit, surface_normal, 0, std::numeric_limits<float>::infinity())) {
        // Use normal shading for objects
        return (Color(1, 1, 1) + surface_normal) * 0.5f;
    }
    Vector3 unit_direction = unit_vector(ray.direction);
    float k = 0.5 * (unit_direction.y + 1.0);
    return Color(1, 1, 1) * (1.0f - k) + Color(0.5, 0.7, 1.0) * k;
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
    
    // World
    World world;
    Sphere sphere(Point3(0, 0, -1), 0.5);
    Sphere ground(Point3(0, -100.5, -1), 100);
    world.add(sphere);
    world.add(ground);
    
    // Camera
    float viewport_height = 2;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1;
    // Now we essentially make the vector plane of our viewport where we will
    // cast our ray
    Point3 origin = Point3(0, 0, 0);
    Vector3 horizontal = Vector3(viewport_width, 0, 0);
    Vector3 vertical  = Vector3(0, viewport_height, 0);
    Point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);
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
            float u = float(i) / (image_width - 1);
            float v = float(j) / (image_height - 1);
            // Cast our ray
            Ray ray(origin, lower_left_corner + horizontal * u + vertical * v - origin);
            // Make our colors
            Color color = ray_color(ray, world);
            // Place in our buffer
            buffer[pos] = color.pixel();
            // Move to our next pixel set
            pos += 1;
        }
    }
    return buffer;
}
