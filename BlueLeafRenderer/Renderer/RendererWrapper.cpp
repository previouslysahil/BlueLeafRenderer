//
//  RendererWrapper.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/25/22.
//

#include "RendererWrapper.h"
#include "Renderer.hpp"

extern "C" Renderer* Renderer_init(int width, int height, int samples_per_pixel, int max_bounces) {
    return new Renderer(width, height, samples_per_pixel, max_bounces);
}

extern "C" void Renderer_deinit(struct Renderer* obj) {
    // Deallocate any objects, bvhs, materials first
    obj->deallocate_scene();
    delete obj;
}

extern "C" void Renderer_render_buffer(struct Renderer* obj, uint32_t* buffer, int curr_sample) {
    return obj->render_buffer(buffer, curr_sample);
}

extern "C" void Renderer_render_buffer_tiled(struct Renderer* obj, uint32_t* buffer, int curr_sample, int tile_size, int tile_h, int tile_w, int h_rm, int w_rm) {
    return obj->render_buffer_tiled(buffer, curr_sample, tile_size, tile_h, tile_w, h_rm, w_rm);
}
