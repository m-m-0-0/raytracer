//
// Created by molar on 28/08/2023.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H


#include "Vector3.h"

class Ray {
public:
    Vector3 Origin;
    Vector3 Direction;
    Vector3 DirectionInverse;

    Ray(){
        Origin = Vector3();
        Direction = Vector3();
        DirectionInverse = Vector3();
    }

    Vector3 at(double t) const{
        return Origin + Direction * t;
    }

    Ray(Vector3 origin, Vector3 direction){
        this->Origin = origin;
        this->Direction = direction;
        this->DirectionInverse = Vector3(1 / direction.x, 1 / direction.y, 1 / direction.z);
    }
};


#endif //RAYTRACER_RAY_H
