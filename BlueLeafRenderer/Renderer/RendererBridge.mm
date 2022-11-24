//
//  RendererBridge.m
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#import <Foundation/Foundation.h>

#import "RendererBridge.h"
#import "Renderer.hpp"

@implementation RendererBridge
/// Translates our c++ render buffer into objective c for use in Swift
/// - Parameters:
///   - width: the image width
///   - height: the image height
- (uint32_t*) renderBuffer:(int) width by:(int) height {
    Renderer renderer;
    // Makes the render buffer with the hard coded image params
    uint32_t* buffer = renderer.render_buffer(width, height);
    return buffer;
}
@end
