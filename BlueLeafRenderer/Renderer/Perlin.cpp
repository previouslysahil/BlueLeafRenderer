//
//  Perlin.cpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/26/22.
//

#include "Perlin.hpp"

#include "Utility.hpp"
#include "LinearAlgebra.hpp"

Perlin::Perlin() {
    ranvec = new Vector3[point_count];
    for (int i = 0; i < point_count; i++) {
        ranvec[i] = unit_vector(random(-1, 1));
    }
    
    perm_x = perlin_generate_perm();
    perm_y = perlin_generate_perm();
    perm_z = perlin_generate_perm();
}

Perlin::~Perlin() {
    delete[] ranvec;
    delete[] perm_x;
    delete[] perm_y;
    delete[] perm_z;
}

/// VOODOO MAGIC!! (get the theme yet) but basically this is just doing
/// multiple noise calls and accumulating that noise to make an even fancier
/// noise texture
/// - Parameters:
///   - point_of_hit: The point of hit of our ray on our object
///   - depth: The amount of noise we are accumulating
double Perlin::turb(const Point3& point_of_hit, int depth) const {
    auto accum = 0.0;
    auto temp_point_of_hit = point_of_hit;
    auto weight = 1.0;
    
    for (int i = 0; i < depth; i++) {
        accum += weight * noise(temp_point_of_hit);
        weight *= 0.5;
        temp_point_of_hit = temp_point_of_hit * 2;
    }
    
    return fabs(accum);
}

/// This method is basically voodoo magic and understanding it isn't worth the
/// trouble IMO but bascially it takes the point of hit and maps that to a double
/// value that represents the "noise" value of at this point
/// - Parameter point_of_hit: The point of hit of our ray on our object
double Perlin::noise(const Point3& point_of_hit) const {
    double u = point_of_hit.x - floor(point_of_hit.x);
    double v = point_of_hit.y - floor(point_of_hit.y);
    double w = point_of_hit.z - floor(point_of_hit.z);
    
    int i = static_cast<int>(floor(point_of_hit.x));
    int j = static_cast<int>(floor(point_of_hit.y));
    int k = static_cast<int>(floor(point_of_hit.z));
    Vector3 c[2][2][2];
    
    for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
            for (int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
            }
        }
    }
    
    return perlin_interp(c, u, v, w);
}

/// Also just not worth comprehending
int* Perlin::perlin_generate_perm() {
    int* p = new int[point_count];
    for (int i = 0; i < point_count; i++) {
        p[i] = i;
    }
    
    permute(p, point_count);
    return p;
}

/// Also also not worth comprehending
/// - Parameters:
///   - p: Who knows
///   - n: Also who knows!
void Perlin::permute(int* p, int n) {
    for (int i = n - 1; i > 0; i--) {
        int target = random_int(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

/// Some even more crazy voodoo magic not worth comprehending
/// - Parameters:
///   - u: Some coordinate
///   - v: Some other coordinate
///   - w: Some other coordinate
double Perlin::perlin_interp(Vector3 c[2][2][2], double u, double v, double w) {
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    double accum = 0.0;
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                Vector3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu))
                * (j * vv + (1 - j) * (1 - vv))
                * (k * ww + (1 - k) * (1 - ww))
                * dot(c[i][j][k], weight_v);
            }
        }
    }
    
    return accum;
}
