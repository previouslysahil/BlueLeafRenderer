//
//  RendererWrapper.h
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/25/22.
//

#ifndef RendererWrapper_hpp
#define RendererWrapper_hpp

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    struct Renderer;
    struct Renderer* Renderer_init(int width, int height, int samples_per_pixel, int max_bounces);
    void Renderer_deinit(struct Renderer* obj);
    uint32_t* Renderer_render_buffer(struct Renderer* obj, int curr_sample);
#ifdef __cplusplus
}
#endif


#endif /* RendererWrapper_h */
