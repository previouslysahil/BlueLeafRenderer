//
//  Perlin.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/26/22.
//

#ifndef Perlin_hpp
#define Perlin_hpp

#include "Vector3.hpp"

class Perlin {
public:
    Perlin();
    ~Perlin();
    
    double noise(const Point3& point_of_hit) const;
    double turb(const Point3& p, int depth = 7) const;
private:
    static const int point_count = 256;
    Vector3* ranvec;
    int* perm_x;
    int* perm_y;
    int* perm_z;

    static int* perlin_generate_perm();

    static void permute(int* p, int n);
    static double perlin_interp(Vector3 c[2][2][2], double u, double v, double w);
};

#endif /* Perlin_hpp */
