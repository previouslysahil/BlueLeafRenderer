//
//  Renderer.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdint.h>

class Renderer {
public:
    uint32_t* render_buffer(int width, int height);
};

#endif /* Renderer_hpp */
