//
//  Color.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/24/22.
//

#ifndef Color_hpp
#define Color_hpp

#include <stdint.h>

class Color {
public:
    float r;
    float g;
    float b;
    float a;
public:
    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    
    uint32_t pixel() const;
};

#endif /* Color_hpp */
