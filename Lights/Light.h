//
// Created by molar on 02/09/2023.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H


#include "../Types/Vector3.h"
#include "../Types/Transform.h"

enum ELightType {
    NONE,
    POINTLIGHT,
    DIRECTIONALLIGHT
};

class Light {
protected:
    ELightType type;
public:
    Transform Transform;
    double intensity;
    Vector3 color;

    Light(double intensity, Vector3 color){
        this->intensity = intensity;
        this->color = color;
        this->type = NONE;
    }

    Light(double intensity){
        this->intensity = intensity;
        this->color = Vector3(1, 1, 1);
        this->type = NONE;
    }

    Light(){
        this->intensity = 1;
        this->color = Vector3(1, 1, 1);
        this->type = NONE;
    }

    ELightType get_type(){
        return type;
    }
};

#endif //RAYTRACER_LIGHT_H
