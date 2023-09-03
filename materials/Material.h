//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H


#include "../Types/Vector3.h"
#include "../Types/Ray.h"
#include "../Types/Hit.h"

class Material {
public:
    Vector3 color;

    Material(Vector3 color){
        this->color = color;
    }

    virtual bool scatter(Ray ray, Hit hit, Vector3 &attenuation, Ray &scattered){
        return false;
    }
};


#endif //RAYTRACER_MATERIAL_H
