//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_AAB_H
#define RAYTRACER_AAB_H


#include "SceneObject.h"

class AAB : public SceneObject {
public:
    double x, y, z;

    AAB(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    AAB(double x, double y, double z, Vector3 position){
        this->x = x;
        this->y = y;
        this->z = z;
        Transform.set_position(position);
    }

    Bounds get_bounds() override;
    bool intersect(Ray ray, Interval dist, Hit &hit) override;
    Vector3 get_normal(Ray ray, double t) override;
};


#endif //RAYTRACER_AAB_H
