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
#include "World.hpp"
#include "Camera.hpp"

#include <stdint.h>
#include <vector>

class Renderer {
public:
    std::vector<Color> pixel_colors;
    int width;
    int height;
    int samples_per_pixel;
    
private:
    int max_bounces;
    int buffer_pos;
    World world;
    Camera camera;
public:
    Renderer(int width, int height, int samples_per_pixel);
    uint32_t* render_buffer();
private:
    Color ray_color(const Ray& ray, World& world, int bounces) const;
};

#endif /* Renderer_hpp */
