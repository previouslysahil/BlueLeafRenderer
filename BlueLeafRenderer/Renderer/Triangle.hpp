//
//  Triangle.hpp
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 12/26/22.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include "Object.hpp"
#include "Vector3.hpp"

class Triangle: public Object {
public:
    Triangle();
    Triangle(Point3 vertex0, Point3 vertex1, Point3 vertex2, Material* material);
    
    virtual bool hit(const Ray& ray, ObjectInfo& info, double t_min, double t_max) override;
    virtual AABB make_bounding_box() const override;
private:
    Point3 vertex0;
    Point3 vertex1;
    Point3 vertex2;
};

#endif /* Triangle_hpp */
