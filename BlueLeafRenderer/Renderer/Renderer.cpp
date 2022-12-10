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
    camera(double(width), double(height), Point3(3, 1.5, 3.5), Point3(0.12, -0.05, -1), Vector3(0, 1, 0), 20, 0.1)
{
    // Our materials
    Lambertian* mat_ground = new Lambertian(Color(0.3, 0.2, 0.8));
    Lambertian* mat_diffuse = new Lambertian(Color(0.86, 0.84, 0.943));
    Dielectric* mat_glass = new Dielectric(Color(0.95, 0.97, 1), 1.5);
    Metal* mat_metal = new Metal(Color(0.9, 0.6, 0.5), 0.1);
    // Add to scene so we can manage memory properly
    scene.add(mat_ground);
    scene.add(mat_diffuse);
    scene.add(mat_glass);
    scene.add(mat_metal);
    // Make a bvh for our following code
    BVH bvh;
    // Make a giant cube of circles to test our bvh
    int count = 10;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            for (int w = 0; w < count; w++) {
                double rand_x = random_double(-0.03, 0.03);
                double rand_y = random_double(-0.03, 0.03);
                double rand_z = random_double(-0.03, 0.03);
                double radius = 0.04;
                Sphere* rand_box_circle = new Sphere(Point3(double(i) / double(count) - 0.5 + rand_x, double(j) / double(count) - 0.5 + rand_y + radius, -double(w) / double(count) - 0.5 + rand_z), radius, mat_diffuse);
                bvh.add(rand_box_circle);
            }
        }
    }
    // Add our bvh of all these circles to the scene
    scene.add(bvh);
    // Our non bvh'd objects
    Sphere* ground = new Sphere(Point3(0, -100.5, -1), 100, mat_ground);
    Sphere* left = new Sphere(Point3(-1, 0, -1), 0.5, mat_glass);
    Sphere* left_in = new Sphere(Point3(-1, 0, -1), -0.45, mat_glass);
    Sphere* right = new Sphere(Point3(1, 0, -1), 0.5, mat_metal);
    // Add these objects to our scene
    scene.add(ground);
    scene.add(left);
    scene.add(left_in);
    scene.add(right);
}

/// Deallocates all our heap allocated scene structures
void Renderer::deallocate_scene() {
    // Clean up scene which holds all objects
    // bvhs and materials
    scene.deallocate_objects();
    scene.deallocate_bvhs();
    scene.deallocate_materials();
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
        // ObjectInfo struct for nearest object hit information
        ObjectInfo nearest_object;
        // Find an object to hit
        if (scene.get_nearest_object(curr_ray, nearest_object, 0.001, std::numeric_limits<double>::infinity())) {
            // Properties to be populated by our material scattering function
            // for proper ray casting and color
            Ray scattered_ray;
            Color scattered_attenuation;
            // Now we attempt to scatter our ray using the object's material's
            // scattering function
            if (nearest_object.material->scatter(curr_ray, nearest_object.point_of_hit, nearest_object.surface_normal, nearest_object.front_face, scattered_attenuation, scattered_ray)) {
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
///   - buffer: the render buffer we will fill with our pixel values as 32bit uint
///   - curr_sample: the current sample out of our samples per pixel we are on
void Renderer::render_buffer(uint32_t* buffer, int curr_sample) {
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
            // Map our loop to the position in our render buffer
            buffer_pos = i + width * abs(j - height + 1);
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
        }
    }
//    // Tiled rendering tile_size by tile_size pixel tiles to increase cache hits since
//    // rays next to each other usually hit the same object
//    int tile_size = 256;
//    int h_rm = height % tile_size;
//    int w_rm = width % tile_size;
//    for (int j = height - 1; j >= h_rm - 1; j -= tile_size) {
//        for (int i = 0; i <= width - w_rm; i += tile_size) {
//            for (int m = 0; m < (j == h_rm - 1 ? h_rm : tile_size); m++) {
//                for (int n = 0; n < (i == width - w_rm ? w_rm : tile_size); n++) {
//                    // std::cout << "j: " << j << " m: " << m << " i: " << i << " n: " << n << " h_rm: " << h_rm << " w_rm: " << w_rm << std::endl;
//                    // Map our loop to the position in our render buffer
//                    buffer_pos = (i + n) + width * abs(j - m - height + 1);
//                    // Get the points on our viewing grid we will cast our ray too
//                    // we ad randomness for anti aliasing
//                    double u = (double(i + n) + random_double()) / double(width - 1);
//                    double v = (double(j - m) + random_double()) / double(height - 1);
//                    // Cast our ray from the camera origin to our u,v coordinate
//                    // on the viewport
//                    Ray ray = camera.create_ray(u, v);
//                    // Make our color for this pixel by creating a rolling average using the previous pixel color
//                    // that contains the previously averaged pixels
//                    Color curr_pixel_color = ray_color(ray, scene, max_bounces) * curr_pixel_color_invariant;
//                    // The bias favoring our previous pixels should be much higher than the current pixel
//                    Color prev_pixel_color = pixel_colors[buffer_pos] * prev_pixel_color_invariant;
//                    // Add together to get final result
//                    pixel_colors[buffer_pos] = prev_pixel_color + curr_pixel_color;
//                    // Gamma correct before we send this pixel color to our buffer
//                    Color gammad_pixel_color = sqrt_color(pixel_colors[buffer_pos]);
//                    // Convert our double color values to a single uint32 RGBA variable
//                    buffer[buffer_pos] = gammad_pixel_color.rgba();
//                }
//            }
//        }
//    }
}

/// This function will render our image and generate the image buffer storing the pixel
/// values of our image in one contiguous buffer in the format R G B R G B R G B . . .
/// the rendering is tiled so our buffer will be populated with a new 'tile' of pixels which
/// each time this funcion is called
/// - Parameters:
///   - buffer: the render buffer we will fill with our pixel values as 32bit uint
///   - curr_sample: the current sample out of our samples per pixel we are on
void Renderer::render_buffer_tiled(uint32_t* buffer, int curr_sample, int tile_size, int tile_h, int tile_w, int h_rm, int w_rm) {
    // Set up our buffer position
    int buffer_pos = 0;
    // curr_pixel_color invariant
    double curr_pixel_color_invariant = (1.0 / double(curr_sample));
    // prev_pixel_color invariant
    double prev_pixel_color_invariant = (double(curr_sample - 1.0) / double(curr_sample));
    // Tiled rendering tile_size by tile_size pixel tiles to increase cache hits since
    // rays next to each other usually hit the same object
    for (int m = 0; m < (tile_h == h_rm - 1 ? h_rm : tile_size); m++) {
        for (int n = 0; n < (tile_w == width - w_rm ? w_rm : tile_size); n++) {
            // std::cout << "j: " << j << " m: " << m << " i: " << i << " n: " << n << " h_rm: " << h_rm << " w_rm: " << w_rm << std::endl;
            // Map our loop to the position in our render buffer
            buffer_pos = (tile_w + n) + width * abs(tile_h - m - height + 1);
            // Get the points on our viewing grid we will cast our ray too
            // we ad randomness for anti aliasing
            double u = (double(tile_w + n) + random_double()) / double(width - 1);
            double v = (double(tile_h - m) + random_double()) / double(height - 1);
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
        }
    }
}
