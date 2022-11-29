//
//  Renderer.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "Color.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

#include <stdint.h>
#include <vector>

class Renderer {
public:
    std::vector<Color> pixel_colors;
    int width;
    int height;
    int samples_per_pixel;
    int max_bounces;
    
private:
    Scene scene;
    Camera camera;
public:
    Renderer(int width, int height, int samples_per_pixel, int max_bounces);
    uint32_t* render_buffer(int curr_sample);
private:
    Color ray_color(const Ray& ray, Scene& scene, int bounces) const;
    Color ray_color_seq(const Ray& ray, Scene& scene, int bounces) const;
};

#endif /* Renderer_hpp */
