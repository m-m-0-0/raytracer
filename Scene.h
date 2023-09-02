//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "Objects/SceneObject.h"
#include "BVHNode.h"
#include <functional>


class Scene {
private:
    SceneObject** objects;
    int object_count;
    int bvh_max_depth = 8;

    BVHNode *root;

    //environment sampling function
    //returns a color
    std::function<Vector3(Ray)> env_func;

public:
    Scene(){
        object_count = 0;
        objects = new SceneObject*[0];
        env_func = nullptr;
    }

    int get_object_count();
    SceneObject** get_objects();

    void add_object(SceneObject *object);

    void set_environment(std::function<Vector3(Ray)> func){
        env_func = func;
    }

    virtual Vector3 sample_environment(Ray ray){
        if(env_func){
            return env_func(ray);
        } else {
            return Vector3(0, 0, 0);
        }
    }

    BVHNode* get_root(){
        return root;
    }

    void set_bvh_max_depth(int depth){
        bvh_max_depth = depth;
    }
    void build_bvh();
};

#endif //RAYTRACER_SCENE_H
