//
//  Camera.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#include "Camera.hpp"

Camera::Camera(double image_width, double image_height) {
    double aspect_ratio = image_width / image_height;
    double viewport_height = 2;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1;
    // Now we essentially make the vector plane of our viewport where we will
    // cast our ray
    origin = Point3(0, 0, 0);
    horizontal = Vector3(viewport_width, 0, 0);
    vertical  = Vector3(0, viewport_height, 0);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);
}

Ray Camera::create_ray(double u, double v) const {
    // Cast our ray from our origin to a point on our view
    return Ray(origin, lower_left_corner + horizontal * u + vertical * v - origin);
}
