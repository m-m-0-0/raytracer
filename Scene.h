//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "Objects/SceneObject.h"
#include "BVHNode.h"
#include "Lights/Light.h"
#include <functional>


class Scene {
private:
    SceneObject** objects;
    int object_count;

    Light** lights;
    int light_count;

    bool is_bvh_built = false;
    int bvh_max_depth = 8;
    BVHNode *root;

    //environment sampling function
    //returns a color
    std::function<Vector3(Ray)> env_func;

public:
    std::string sceneName = "";

    Scene(){
        object_count = 0;
        objects = new SceneObject*[0];
        env_func = nullptr;
        sceneName = nullptr;
    }

    explicit Scene(std::string name){
        object_count = 0;
        objects = new SceneObject*[0];
        env_func = nullptr;
        sceneName = name;
    }

    int get_object_count();
    SceneObject** get_objects();
    void add_object(SceneObject *object);

    int get_light_count();
    Light** get_lights();
    void add_light(Light *light);

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
        if(depth != bvh_max_depth) {
            bvh_max_depth = depth;
            is_bvh_built = false;
        }

    }

    void build_bvh();
    int get_bvh_max_depth() const;

    Bounds get_bounds(){
        if(!is_bvh_built){
            build_bvh();
        }
        return root->get_bounds();
    }

    virtual void init_scene() = 0;
};

#endif //RAYTRACER_SCENE_H
