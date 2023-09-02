//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_HIT_H
#define RAYTRACER_HIT_H


#include "Vector3.h"
#include "Ray.h"

class SceneObject;
class Material;

struct Hit {
    Vector3 position;
    Vector3 normal;
    double t;
    bool hit;
    bool front_face;

    Material* material;

    SceneObject* object;

    Hit(){
        t = INFINITY;
        hit = false;
        material = nullptr;
        object = nullptr;
        front_face = false;
    }

    void set_face_normal(Ray ray, Vector3 outward_normal){
        if(ray.Direction.dot(outward_normal) < 0){
            front_face = true;
            normal = outward_normal;
        } else {
            front_face = false;
            normal = -outward_normal;
        }
    }
};


#endif //RAYTRACER_HIT_H
