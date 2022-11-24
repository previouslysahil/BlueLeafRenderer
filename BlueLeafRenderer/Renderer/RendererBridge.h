//
//  RendererBridge.h
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef RendererWrapper_h
#define RendererWrapper_h

#import <Foundation/Foundation.h>

@interface RendererBridge: NSObject
- (uint32_t*) renderBuffer:(int) width by:(int) height;
@end


#endif /* RendererWrapper_h */
