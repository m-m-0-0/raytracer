//
// Created by molar on 31/08/2023.
//

#ifndef RAYTRACER_VOLUMETEST_H
#define RAYTRACER_VOLUMETEST_H


#include "../Ray.h"
#include "../Hit.h"
#include "../Material.h"

class VolumeTest : public Material{
public:
    double probability;
    double absorption;

    VolumeTest(Vector3 color, double probability, double absorption) : Material(color) {
        this->probability = probability;
        this->absorption = absorption;
    };

    bool scatter(Ray ray, Hit hit, Vector3 &attenuation, Ray &scattered) override {
        if(random_double() < probability) {
            scattered = Ray(hit.position, Vector3::random_in_unit_sphere());
            attenuation = color * absorption;
            return true;
        }
        scattered = Ray(hit.position, ray.Direction);
        attenuation = color;
        return true;
    }
};


#endif //RAYTRACER_VOLUMETEST_H
