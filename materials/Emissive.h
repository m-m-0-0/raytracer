//
// Created by molar on 30/08/2023.
//

#ifndef RAYTRACER_EMISSIVE_H
#define RAYTRACER_EMISSIVE_H


#include "../Material.h"

class Emissive : public Material{
public:
    double intensity;

    Emissive(Vector3 color, double intensity) : Material(color) {
        this->intensity = intensity;
    }

    bool scatter(Ray ray, Hit hit, Vector3 &attenuation, Ray &scattered) override {
        scattered = Ray(hit.position, hit.normal);
        attenuation = color * intensity;
        return true;
    }
};


#endif //RAYTRACER_EMISSIVE_H
