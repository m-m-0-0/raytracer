//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_SCENEOBJECT_H
#define RAYTRACER_SCENEOBJECT_H

#include "../Types/Ray.h"
#include "../Types/Transform.h"
#include "../Types/Interval.h"
#include "../Types/Hit.h"
#include "../materials/Material.h"
#include "../Types/Bounds.h"

class SceneObject {
public:
    Transform Transform;
    Material* material;
    Bounds* bounds = nullptr;

    SceneObject(){
        Transform.set_position(Vector3());
        material = new Material(Vector3(.5, .5, .5));
    }

    void move_at(Vector3 position){
        Transform.set_position(position);
    }

    void move_by(Vector3 delta){
        Transform.set_position(Transform.position() + delta);
    }

    virtual Bounds* get_bounds(){
        return new Bounds(Transform.position(), Vector3(0, 0, 0));
    }


    virtual bool intersect(Ray ray, Interval dist, Hit &hit){
        return false;
    }

    virtual Vector3 get_normal(Ray ray, double t) {
        return {};
    }

    virtual Vector3 get_color(){
        return material->color;
    }

    virtual void set_color(Vector3 color){
        material->color = color;
    }
};


#endif //RAYTRACER_SCENEOBJECT_H
