//
// Created by molar on 30/08/2023.
//

#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H


#include "Material.h"

class Lambertian : public Material{
public:
    Lambertian(Vector3 color) : Material(color) {}

    bool scatter(Ray ray, Hit hit, Vector3 &attenuation, Ray &scattered) override {
        auto direction = hit.normal + Vector3::random_unit_vector();
        if(direction.near_zero()){
            direction = hit.normal;
        }
        scattered = Ray(hit.position, direction);
        attenuation = color;

        return true;
    }
};

#endif //RAYTRACER_LAMBERTIAN_H
