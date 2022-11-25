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
    double r;
    double g;
    double b;
    double a;
public:
    Color();
    Color(double r, double g, double b);
    Color(double r, double g, double b, double a);
    
    uint32_t rgba() const;
};

#endif /* Color_hpp */
