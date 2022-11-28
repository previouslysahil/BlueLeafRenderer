//
//  Renderer.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

#include "Renderer.hpp"
#include "Object.hpp"
#include "Utility.hpp"

#include <cmath>
#include <limits>
#include <iostream>

/// This method will set up our renderer along side generating our scene, camera and the bounces
/// and max samples of our image
/// - Parameters:
///   - width: The width of our rendered image
///   - height: The height of our rendered image
///   - samples_per_pixel: The number of samples to run on every pixel
///   - max_bounces: The maximum number of bounces for each ray
Renderer::Renderer(int width, int height, int samples_per_pixel, int max_bounces):
    pixel_colors(width * height, Color(0, 0, 0)),
    width(width), height(height),
    samples_per_pixel(samples_per_pixel),
    max_bounces(max_bounces),
    scene(),
    camera(double(width), double(height), Point3(-2, 2, 1), Point3(0, 0, -1), Vector3(0, 1, 0), 90)
{
    // Our materials
    Material mat_ground = Material(Color(0.8, 0.8, 0), Lambertian);
    Material mat_center = Material(Color(0.1, 0.2, 0.5), Lambertian);
    Material mat_left = Material(1.5);
    Material mat_right = Material(Color(0.8, 0.6, 0.2), 0);
    // Our objects
    Object ground(Point3(0, -100.5, -1), 100, mat_ground);
    Object center(Point3(0, 0, -1), 0.5, mat_center);
    Object left(Point3(-1, 0, -1), 0.5, mat_left);
    Object left_in(Point3(-1, 0, -1), -0.4, mat_left);
    Object right(Point3(1, 0, -1), 0.5, mat_right);
    // scene is default init'd so no need to init
    scene.add(ground);
    scene.add(center);
    scene.add(left);
    scene.add(left_in);
    scene.add(right);
}

/// Makes our color at the point of intersection of our ray
/// - Parameter r: Ray cast from origin to direction
Color Renderer::ray_color(const Ray& ray, Scene& scene, int bounces) const {
    // Only recurse our ray bounces until we have reached our limit
    if (bounces <= 0) {
        // If we reach our bounce limit the ray is basically absorbed
        // so we return black!
        return Color(0, 0, 0);
    }
    // Used to calculate our new ray and color
    Point3 point_of_hit;
    Vector3 surface_normal;
    bool front_face;
    Material object_material;
    // Find an object to hit
    if (scene.findNearestObject(ray, point_of_hit, surface_normal, front_face, object_material, 0.001, std::numeric_limits<double>::infinity())) {
        // Properties to be populated by our material scattering function
        // for proper ray casting and color
        Ray scattered_ray;
        Color attenuation;
        // Now we attempt to scatter our ray using the object's material's
        // scattering function
        if (object_material.scatter(ray, point_of_hit, surface_normal, front_face, attenuation, scattered_ray)) {
            // We use our attenuation (which is the color of our material) to color our ray
            return attenuation * ray_color(scattered_ray, scene, bounces - 1);
        }
        // Unable to scatter this ray so return black because our ray
        // is basically absorbed
        return Color(0, 0, 0);
    }
    // Otherwise our ray color is the sky
    Vector3 unit_direction = unit_vector(ray.direction);
    double k = 0.5 * (unit_direction.y + 1.0);
    return Color(1, 1, 1) * (1.0 - k) + Color(0.5, 0.7, 1.0) * k;
}

/// This function will render our image andgenerate the image buffer storing the pixel
/// values of our image in one contiguous buffer in the format R G B R G B R G B . . .
/// - Parameters:
///   - curr_sample: the current sample out of our samples per pixel we are on
uint32_t* Renderer::render_buffer(int curr_sample) {
    // Create our buffer with enough size for all our pixels
    // THIS IS FREED IN Renderer.swift
    uint32_t* buffer = new uint32_t[width * height];
    // Set up our buffer position
    int buffer_pos = 0;
    // Generate image top down, left right so buffer stores pixel values
    // in their proper spot
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            // Get the points on our viewing grid we will cast our ray too
            // we ad randomness for anti aliasing
            double u = (double(i) + random_double()) / double(width - 1);
            double v = (double(j) + random_double()) / double(height - 1);
            // Cast our ray from the camera origin to our u,v coordinate
            // on the viewport
            Ray ray = camera.create_ray(u, v);
            // Make our color for this pixel by creating a rolling average using the previous pixel color
            // that contains the previously averaged pixels
            Color curr_pixel_color = ray_color(ray, scene, max_bounces) * (1.0 / double(curr_sample));
            // The bias favoring our previous pixels should be much higher than the current pixel
            Color prev_pixel_color = pixel_colors[buffer_pos] * (double(curr_sample - 1.0) / double(curr_sample));
            // Add together to get final result
            pixel_colors[buffer_pos] = prev_pixel_color + curr_pixel_color;
            // Gamma correct before we send this pixel color to our buffer
            Color gammad_pixel_color = sqrt_color(pixel_colors[buffer_pos]);
            // Convert our double color values to a single uint32 RGBA variable
            buffer[buffer_pos] = gammad_pixel_color.rgba();
            // Move to our next pixel set
            buffer_pos += 1;
        }
    }
    return buffer;
}
