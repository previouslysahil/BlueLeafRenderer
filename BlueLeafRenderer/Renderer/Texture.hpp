//
//  Texture.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/23/22.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "Color.hpp"
#include "Vector3.hpp"

class Texture {
public:
    Texture();
    virtual ~Texture() = default;
    virtual Color value(double u, double v, const Point3& point_of_hit) const = 0;
};

class SolidColor: public Texture {
public:
    SolidColor();
    SolidColor(Color color_value);
    
    SolidColor(double red, double green, double blue);
    
    virtual Color value(double u, double v, const Point3& point_of_hit) const override;
    
private:
    Color color_value;
};

class CheckerTexture: public Texture {
public:
    CheckerTexture();
    CheckerTexture(SolidColor* color1, SolidColor* color2, double scale);
    
    virtual Color value(double u, double v, const Point3& point_of_hit) const override;
private:
    SolidColor* odd;
    SolidColor* even;
    double scale;
};


#endif /* Texture_hpp */
