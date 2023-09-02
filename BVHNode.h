//
// Created by molar on 31/08/2023.
//

#ifndef RAYTRACER_BVHNODE_H
#define RAYTRACER_BVHNODE_H


#include "Bounds.h"
#include "Objects/SceneObject.h"
#include <unordered_set>

class BVHNode {
private:
    int depth = 0;
    Bounds bounds;
    BVHNode *parent;
    BVHNode **children;

    SceneObject** objects;
    int object_count;

public:
    int max_depth = 8;

    BVHNode(Bounds bounds, BVHNode *parent){
        this->bounds = bounds;
        this->parent = parent;
        this->children = nullptr;
        this->objects = nullptr;
        this->object_count = 0;
    }

    BVHNode(Bounds bounds){
        this->bounds = bounds;
        this->parent = nullptr;
        this->children = nullptr;
        this->objects = nullptr;
        this->object_count = 0;
    }

    explicit BVHNode(){
        this->bounds = Bounds();
        this->parent = nullptr;
        this->children = nullptr;
        this->objects = nullptr;
        this->object_count = 0;
    }

    void add_object(SceneObject* object){
        if (objects == nullptr){
            objects = new SceneObject*[1];
            objects[0] = object;
            object_count = 1;
            bounds.encapsulate(object->get_bounds());
            return;
        }

        auto** new_objects = new SceneObject*[object_count + 1];

        for (int i = 0; i < object_count; i++){
            new_objects[i] = objects[i];
        }

        bounds.encapsulate(object->get_bounds());

        new_objects[object_count++] = object;
        delete[] objects;
        objects = new_objects;
    }

    void build(int _depth);
    void shake();
    void get_intersecting(Ray ray, std::unordered_set<SceneObject *> &_objects);

    void recalculate_bounds();
};


#endif //RAYTRACER_BVHNODE_H
