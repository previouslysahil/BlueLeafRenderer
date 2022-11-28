//
//  Camera.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "Vector3.hpp"
#include "Ray.hpp"

class Camera {
private:
    Point3 origin;
    Point3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;
    
public:
    Camera(double image_width, double image_height, Point3 lookfrom, Point3 lookat, Vector3 vup, double vfov);
    Ray create_ray(double h, double v) const;
};

#endif /* Camera_hpp */
