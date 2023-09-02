//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H


#include "SceneObject.h"
#include "../Interval.h"

class Sphere : public SceneObject {
public:
    double radius;

    Sphere(double radius){
        this->radius = radius;
    }

    Sphere(double radius, Vector3 position){
        this->radius = radius;
        Transform.set_position(position);
    }

    Bounds get_bounds() override;
    bool intersect(Ray ray, Interval dist, Hit &hit) override;
    Vector3 get_normal(Ray ray, double t) override;
};


#endif //RAYTRACER_SPHERE_H
