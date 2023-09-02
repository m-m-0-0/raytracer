//
// Created by molar on 30/08/2023.
//

#ifndef RAYTRACER_METALLIC_H
#define RAYTRACER_METALLIC_H


#include "../Material.h"

class Metallic : public Material {
public:
    double fuzz;
    Metallic(Vector3 color) : Material(color) {}
    Metallic(Vector3 color, double fuzz) : Material(color) {
        this->fuzz = fuzz;
    }

    bool scatter(Ray ray, Hit hit, Vector3 &attenuation, Ray &scattered) override {
        auto reflected = ray.Direction.reflect(hit.normal);
        scattered = Ray(hit.position, reflected + fuzz * Vector3::random_unit_vector());
        attenuation = color;
        return true;
    }
};


#endif //RAYTRACER_METALLIC_H
