//
// Created by molar on 29/08/2023.
//

#include "Scene.h"

int Scene::get_object_count() {
    return object_count;
}

SceneObject** Scene::get_objects() {
    return objects;
}

void Scene::add_object(SceneObject *object) {
    is_bvh_built = false;
    auto** new_objects = new SceneObject*[object_count + 1];
    for(int i = 0; i < object_count; i++){
        new_objects[i] = objects[i];
    }
    new_objects[object_count++] = object;
    delete[] objects;
    objects = new_objects;
}

int Scene::get_light_count() {
    return light_count;
}

Light** Scene::get_lights() {
    return lights;
}

void Scene::add_light(Light *light) {
    auto** new_lights = new Light*[light_count + 1];
    for(int i = 0; i < light_count; i++){
        new_lights[i] = lights[i];
    }
    new_lights[light_count++] = light;
    delete[] lights;
    lights = new_lights;
}

void Scene::build_bvh() {
    if(is_bvh_built){
        return;
    }
    std::cout << "Building BVH" << std::endl;
    root = new BVHNode();
    root->max_depth = bvh_max_depth;
    for(int i=0; i<object_count; i++){
        root->add_object(objects[i]);
    }

    root->build(0);
    root->shake();
    //root->recalculate_bounds();
    std::cout << "Done building BVH" << std::endl;
    //root->print_tree(0);
    is_bvh_built = true;
}

int Scene::get_bvh_max_depth() const {
    return bvh_max_depth;
}
