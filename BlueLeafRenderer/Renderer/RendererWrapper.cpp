//
//  RendererWrapper.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/25/22.
//

#include "RendererWrapper.h"
#include "Renderer.hpp"

extern "C" Renderer* Renderer_init(int width, int height, int samples_per_pixel) {
    return new Renderer(width, height, samples_per_pixel);
}

extern "C" void Renderer_deinit(struct Renderer* obj) {
    delete obj;
}

extern "C" uint32_t* Renderer_render_buffer(struct Renderer* obj) {
    return obj->render_buffer();
}
