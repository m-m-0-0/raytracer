//
// Created by molar on 30/08/2023.
//

#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H


#include "../Material.h"

class Dielectric : public Material {
public:
    double refraction_index = 1.5;
    Dielectric(Vector3 color) : Material(color) {};
    Dielectric(Vector3 color, double refraction_index) : Material(color) {
        this->refraction_index = refraction_index;
    };

    bool scatter(Ray ray, Hit hit, Vector3 &attenuation, Ray &scattered) override{
        attenuation = color;
        double refraction_ratio = hit.front_face ? (1.0 / refraction_index) : refraction_index;

        double cos_theta = fmin((-ray.Direction).dot(hit.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if(refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > random_double()){
            scattered = Ray(hit.position, ray.Direction.unit_vector().reflect(hit.normal));
        } else {
            scattered = Ray(hit.position, ray.Direction.unit_vector().refract(hit.normal, refraction_ratio));
        }
        return true;
    }

private:
    static double reflectance(double cos, double ref_i){
        auto r0 = (1 - ref_i) / (1 + ref_i);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cos), 5);
    }
};


#endif //RAYTRACER_DIELECTRIC_H