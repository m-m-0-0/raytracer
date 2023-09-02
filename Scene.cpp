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
    auto** new_objects = new SceneObject*[object_count + 1];
    for(int i = 0; i < object_count; i++){
        new_objects[i] = objects[i];
    }
    new_objects[object_count++] = object;
    delete[] objects;
    objects = new_objects;
}

void Scene::build_bvh() {
    std::cout << "Building BVH" << std::endl;
    root = new BVHNode();
    for(int i=0; i<object_count; i++){
        root->add_object(objects[i]);
    }

    root->build(0);
    root->shake();
    std::cout << "Done building BVH" << std::endl;
}
