//
//  Camera.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Camera.hpp"

/// Creates a camera object and generates a viewport using the
/// dimensions of the image we provide
/// - Parameters:
///   - image_width: The width of our image
///   - image_height: The height of our image
///   - lookfrom: The cameras origin point, where we cast rays from initially
///   - lookat: The center of the viewport plane we are looking at
///   - global_y: Vector orienting our local x and y axis of our camera
///   - vfov: Our vertical field of view
///   - aperture: Decides how much blur should be in our scene
Camera::Camera(double image_width, double image_height, Point3 lookfrom, Point3 lookat, Vector3 global_y, double vfov, double aperture) {
    // Make our aspect ratio for view port width calculation
    double aspect_ratio = image_width / image_height;
    // Convert our vfov to radians for viewport height calculation
    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2.0);
    // Make our viewport height using in vector units using this
    // theta calculation
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;
    // Now compute our orthnormal basis for our camera
    // AKA the local axis of the 'camera' pointed towards
    // the lookat point
    local_z = unit_vector(lookfrom - lookat);
    local_x = unit_vector(cross(global_y, local_z));
    local_y = cross(local_z, local_x);
    // Make our focus dist to mitigate offset in create ray
    double focus_dist = (lookfrom - lookat).length();
    // Now we essentially make the vector plane of our viewport where we will
    // cast our ray
    origin = lookfrom;
    horizontal = local_x * viewport_width * focus_dist;
    vertical  = local_y * viewport_height * focus_dist;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - local_z * focus_dist;
    // Setup our lens radius
    lens_radius = aperture / 2.0;
}

/// Casts a ray from the origin of our camera onto the viewport plane we
/// construct in the camera object
/// - Parameters:
///   - h: The horizontal point we are looking at on our viewport
///   - v: The vertical point we are looking at on our viewport
Ray Camera::create_ray(double h, double v) const {
    // Make random vector whose x and y values will
    // be used to randomize our local_x and local_y
    // these values lie in a disk shape
    Vector3 rand = random_in_unit_disk() * lens_radius;
    // Now we create a vector with a slight random offset
    // that sits in our local camera axis and is based
    // off our lookfrom point
    Vector3 offset = local_x * rand.x + local_y * rand.y;
    // Cast our ray from our origin to a point on our view
    // use our random offset for focus blur
    return Ray(origin + offset, lower_left_corner + horizontal * h + vertical * v - origin - offset);
}
