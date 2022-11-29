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
    camera(double(width), double(height), Point3(3, 1.5, 3.5), Point3(0.12, -0.05, -1), Vector3(0, 1, 0), 20, 0.1, 0.0, 1.0)
{
    // Our materials
    Material mat_ground(Color(0.3, 0.2, 0.8), 0, 0, Lambertian);
    Material mat_center(Color(0.3, 0.08, 0.343), 0, 0, Lambertian);
    Material mat_left(Color(0.95, 0.97, 1), 0, 1.5, Dielectric);
    Material mat_right(Color(0.9, 0.6, 0.5), 0.1, 0, Metal);
    // Our objects
    Object ground(Point3(0, -100.5, -1), 100, mat_ground);
    Object center(Point3(0, 0, -1), 0.5, mat_center);
    Object left(Point3(-1, 0, -1), 0.5, mat_left);
    Object left_in(Point3(-1, 0, -1), -0.45, mat_left);
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
/// - Parameter scene: Scene containing our object
/// - Parameter bounces: The maxium bounces we should calculate
Color Renderer::ray_color(const Ray& ray, Scene& scene, int bounces) const {
    // Values to track the ray and color after each
    // bounce
    Ray curr_ray(ray);
    // Init curr attenuation to 1 which will properly
    // multiply in any situation
    Color curr_attenuation(1, 1, 1);
    // Iterate until bounces, loop may exit early if
    // we are unable to scatter a ray or hit our sky
    for (int i = 0; i < bounces; i++) {
        // Used to calculate our new ray and color
        Point3 point_of_hit;
        Vector3 surface_normal;
        bool front_face;
        Material object_material;
        // Find an object to hit
        if (scene.findNearestObject(curr_ray, point_of_hit, surface_normal, front_face, object_material, 0.001, std::numeric_limits<double>::infinity())) {
            // Properties to be populated by our material scattering function
            // for proper ray casting and color
            Ray scattered_ray;
            Color scattered_attenuation;
            // Now we attempt to scatter our ray using the object's material's
            // scattering function
            if (object_material.scatter(curr_ray, point_of_hit, surface_normal, front_face, scattered_attenuation, scattered_ray)) {
                // We use our scattered attenuation (which is the color of our material)
                // to further color our ray using the existing attenuation
                curr_attenuation = scattered_attenuation * curr_attenuation;
                // We also make set our scattered ray to our curr ray for next iteration
                curr_ray = scattered_ray;
            } else {
                // Unable to scatter this ray so return black because our ray
                // is basically absorbed (no need to multiply by curr_attenuation
                // since it will multiply to black)
                return Color(0, 0, 0);
            }
        } else {
            // Otherwise our ray color is the sky
            Vector3 unit_direction = unit_vector(ray.direction);
            double k = 0.5 * (unit_direction.y + 1.0);
            Color sky_attenuation = Color(1, 1, 1) * (1.0 - k) + Color(0.5, 0.7, 1.0) * k;
            // Use our sky attenuation to further color our ray using the
            // existing attenuation
            curr_attenuation = curr_attenuation * sky_attenuation;
            return curr_attenuation;
        }
    }
    // If we reach our bounce limit the ray is basically absorbed
    // so we return black!
    return Color(0, 0, 0);
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
    // curr_pixel_color invariant
    double curr_pixel_color_invariant = (1.0 / double(curr_sample));
    // prev_pixel_color invariant
    double prev_pixel_color_invariant = (double(curr_sample - 1.0) / double(curr_sample));
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
            Color curr_pixel_color = ray_color(ray, scene, max_bounces) * curr_pixel_color_invariant;
            // The bias favoring our previous pixels should be much higher than the current pixel
            Color prev_pixel_color = pixel_colors[buffer_pos] * prev_pixel_color_invariant;
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
