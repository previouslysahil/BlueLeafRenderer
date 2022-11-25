//
//  Renderer.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

#include "Renderer.hpp"
#include "Sphere.hpp"
#include "Utility.hpp"

#include <cmath>
#include <limits>
#include <iostream>

/// This method will set up our renderer along side generating our scene, and setting up our buffer position
/// for our render buffers multiple sample passes, we set our max bounces to 9 and our buffer position to
/// 0 and make an array of pixel colors that will be used to update our image with each new sample
/// - Parameters:
///   - width: The width of our rendered image
///   - height: The height of our rendered image
///   - samples_per_pixel: The number of samples to run on every pixel
Renderer::Renderer(int width, int height, int samples_per_pixel): pixel_colors(width * height, Color(0, 0, 0)), width(width), height(height), samples_per_pixel(samples_per_pixel), max_bounces(8), buffer_pos(0), world(), camera(static_cast<double>(width), static_cast<double>(height)) {
    Sphere sphere(Point3(0, 0, -1), 0.5);
    Sphere ground(Point3(0, -100.5, -1), 100);
    // world is default init'd so no need to init
    world.add(sphere);
    world.add(ground);
}

/// Makes our color at the point of intersection of our ray
/// - Parameter r: Ray cast from origin to direction
Color Renderer::ray_color(const Ray& ray, World& world, int bounces) const {
    // Only recurse our ray bounces until we have reached our limit
    if (bounces <= 0) {
        return Color(0, 0, 0);
    }
    Point3 point_of_hit;
    Vector3 surface_normal;
    if (world.findNearestObject(ray, point_of_hit, surface_normal, 0, std::numeric_limits<double>::infinity())) {
        // Find a target point that is on the surface of the unit sphere of
        // our surface normal
        Point3 target = point_of_hit + surface_normal + random_unit_vector();
        // Cast a new ray from our inital hit point to this new target point
        // in the unit sphere of our surface normal
        return ray_color(Ray(point_of_hit, target - point_of_hit), world, bounces - 1);
    }
    Vector3 unit_direction = unit_vector(ray.direction);
    double k = 0.5 * (unit_direction.y + 1.0);
    return Color(1, 1, 1) * (1.0 - k) + Color(0.5, 0.7, 1.0) * k;
}

/// This function will generate the image buffer storing the pixel values of our image
/// in one contiguous buffer in the format R G B R G B R G B . . .
/// - Parameters:
///   - width: the image width
///   - height: the image height
uint32_t* Renderer::render_buffer() {
    // Create our buffer with enough size for all our pixels
    uint32_t* buffer = new uint32_t[width * height];
    // Generate image top down, left right so buffer stores pixel values
    // in their proper spot
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            for (int s = 0; s < samples_per_pixel; s++) {
                // Get the points on our viewing grid we will cast our ray too
                // we ad randomness for anti aliasing
                double u = (double(i) + random_double()) / (width - 1);
                double v = (double(j) + random_double()) / (height - 1);
                // Cast our ray from the camera origin to our u,v coordinate
                // the viewport
                Ray ray = camera.create_ray(u, v);
                // Make our color for this pixel by accumulating all of
                // our different ray samples from our camera to the view
                pixel_colors[buffer_pos] = pixel_colors[buffer_pos] + ray_color(ray, world, max_bounces);
            }
            // Average our accumulated pixel color
            pixel_colors[buffer_pos] = pixel_colors[buffer_pos] / double(samples_per_pixel);
            // Gamma correct
            pixel_colors[buffer_pos] = sqrt_color(pixel_colors[buffer_pos]);
            // Convert our double color values to a single uint32 RGBA variable
            buffer[buffer_pos] = pixel_colors[buffer_pos].rgba();
            // Move to our next pixel set
            buffer_pos += 1;
        }
    }
    return buffer;
}
