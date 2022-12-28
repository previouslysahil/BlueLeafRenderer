//
//  Renderer.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

#include "Renderer.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
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
    // Our textures (just solid colors for now)
    SolidColor* tex_ground_1 = new SolidColor(0.99, 0.98, 0.96);
    SolidColor* tex_ground_2 = new SolidColor(0.1, 0.11, 0.13);
    CheckerTexture* tex_ground = new CheckerTexture(tex_ground_1, tex_ground_2, 5);
    SolidColor* tex_diffuse = new SolidColor(0.938, 0.854, 0.793);
    SolidColor* tex_glass = new SolidColor(0.891, 0.861, 0.975);
    NoiseTexture* tex_metal = new NoiseTexture(5);
    SolidColor* tex_triangle = new SolidColor(0.879, 0.441, 0.194);
    SolidColor* tex_triangle_diffuse = new SolidColor(10, 8, 6);
    // Add to scene so we can manage memory properly
    scene.add(tex_ground_1);
    scene.add(tex_ground_2);
    scene.add(tex_ground);
    scene.add(tex_diffuse);
    scene.add(tex_glass);
    scene.add(tex_metal);
    scene.add(tex_triangle);
    scene.add(tex_triangle_diffuse);
    // Our materials
    Lambertian* mat_ground = new Lambertian(tex_ground);
    Lambertian* mat_diffuse = new Lambertian(tex_diffuse);
    Dielectric* mat_glass = new Dielectric(tex_glass, 1.5);
    Metal* mat_metal = new Metal(tex_metal, 0.1);
    Lambertian* mat_triangle = new Lambertian(tex_triangle);
    DiffuseLight* mat_triangle_diffuse = new DiffuseLight(tex_triangle_diffuse);
    // Add to scene so we can manage memory properly
    scene.add(mat_ground);
    scene.add(mat_diffuse);
    scene.add(mat_glass);
    scene.add(mat_metal);
    scene.add(mat_triangle);
    scene.add(mat_triangle_diffuse);
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
    Sphere* left = new Sphere(Point3(-1, 0, -0.9), 0.5, mat_glass);
    Sphere* left_in = new Sphere(Point3(-1, 0, -0.9), -0.45, mat_glass);
    Sphere* right = new Sphere(Point3(1, 0, -1), 0.5, mat_metal);
    Triangle* tri = new Triangle(Point3(-0.5, -0.5, -0.4), Point3(-0.5, 0.5, -0.4), Point3(0.5, 0.5, -0.4), mat_triangle);
    Triangle* tri2 = new Triangle(Point3(0.5, -0.5, -0.4), Point3(0.5, 0.5, -0.4), Point3(-0.5, -0.5, -0.4), mat_triangle);
    Triangle* tri3 = new Triangle(Point3(-0.5, 1, -0.5 + 0.5), Point3(-0.5, 1, -1.5 + 0.5), Point3(0.5, 1, -1.5 + 0.5), mat_triangle_diffuse);
    Triangle* tri4 = new Triangle(Point3(0.5, 1, -1.5 + 0.5), Point3(0.5, 1, -0.5 + 0.5), Point3(-0.5, 1, -0.5 + 0.5), mat_triangle_diffuse);
    // Add these objects to our scene
    scene.add(ground);
    scene.add(left);
    scene.add(left_in);
    scene.add(right);
    scene.add(tri);
    scene.add(tri2);
    scene.add(tri3);
    scene.add(tri4);
}

/// Deallocates all our heap allocated scene structures
void Renderer::deallocate_scene() {
    // Clean up scene which holds all objects
    // bvhs and materials
    scene.deallocate_objects();
    scene.deallocate_bvhs();
    scene.deallocate_materials();
    scene.deallocate_textures();
}

/// Makes our color at the point of intersection of our ray
/// - Parameter r: Ray cast from origin to direction
/// - Parameter scene: Scene containing our object
/// - Parameter bounces: The maxium bounces we should calculate
Color Renderer::ray_color(const Ray& ray, Scene& scene, int bounces) const {
    // Bounces reached, ray absorbed so return black (no color)
    if (bounces <= 0) {
        return Color(0, 0, 0);
    }
    // ObjectInfo struct for nearest object hit information
    ObjectInfo nearest_object;
    if (!scene.get_nearest_object(ray, nearest_object, 0.001, std::numeric_limits<double>::infinity())) {
        return Color(0, 0, 0);
    }
    // Properties to be populated by our material scattering function
    // for proper ray casting and color
    Ray scattered_ray;
    Color scattered_attenuation;
    // Check if we have an light emission from this object (will be 0 for normal objects)
    Color emitted_light = nearest_object.material->emitted(nearest_object.u, nearest_object.v, nearest_object.point_of_hit);
    // If we don't have any scattering from this object then we only return the light emission (will hold true for diffuse light objects)
    if (!nearest_object.material->scatter(ray, nearest_object, scattered_attenuation, scattered_ray)) {
        return emitted_light;
    }
    // Else we recurse and bounce our ray one more time adding our emitted light color to our
    // color that will be computed from all the ray bounce scatterings to occur
    return emitted_light + scattered_attenuation * ray_color(scattered_ray, scene, bounces - 1);
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
