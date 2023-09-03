//
// Created by molar on 02/09/2023.
//

#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H

#include "Light.h"

class PointLight : public Light {
public:
    PointLight(double intensity, Vector3 color) : Light(intensity, color){
        this->type = POINTLIGHT;
    }

    PointLight(double intensity) : Light(intensity){
        this->type = POINTLIGHT;
    }

    PointLight() : Light(){
        this->type = POINTLIGHT;
    }
};


#endif //RAYTRACER_POINTLIGHT_H
