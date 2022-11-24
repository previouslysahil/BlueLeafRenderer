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

#include <stdint.h>

class Renderer {
public:
    uint32_t* render_buffer(int width, int height) const;
private:
    Color ray_color(const Ray& r) const;
};

#endif /* Renderer_hpp */
