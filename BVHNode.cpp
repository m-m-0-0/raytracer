//
// Created by molar on 31/08/2023.
//

#include "BVHNode.h"
#include <vector>
#include <algorithm>
#include <unordered_set>

void BVHNode::recalculate_bounds() {
    bounds = Bounds();
    if(objects != nullptr) {
        for (int i = 0; i < object_count; i++) {
            bounds.encapsulate(objects[i]->get_bounds());
        }
    }

    if(children != nullptr){
        for(int i=0; i<2; i++){
            if(children[i] == nullptr){
                continue;
            }
            bounds.encapsulate(children[i]->bounds);
        }
    }
}

void BVHNode::build(int _depth) {
    if(children == nullptr && objects == nullptr){
        return;
    }
    if (children == nullptr && objects != nullptr) {
        if (object_count > 0) {
            if (_depth < max_depth) {
                //we split!
                children = new BVHNode*[2]; //octree

                Vector3 extents = bounds.extents;
                Vector3 center = bounds.center;

                //split on longest axis

                int axis = 0;
                if (extents.y > extents.x && extents.y > extents.z)
                    axis = 1;
                else if (extents.z > extents.y && extents.z > extents.x)
                    axis = 2;

                Vector3 left = center;
                Vector3 right = center;
                Vector3 new_extents = extents;

                if (axis == 0) {
                    left.x -= extents.x / 2;
                    right.x += extents.x / 2;
                    new_extents.x *= 0.51;
                } else if (axis == 1) {
                    left.y -= extents.y / 2;
                    right.y += extents.y / 2;
                    new_extents.y *= 0.51;
                } else {
                    left.z -= extents.z / 2;
                    right.z += extents.z / 2;
                    new_extents.z *= 0.51;
                }

                children[0] = new BVHNode(Bounds(left, new_extents), this);
                children[1] = new BVHNode(Bounds(right, new_extents), this);

                for (int i = 0; i < 2; i++) {
                    children[i]->depth = _depth + 1;
                    children[i]->max_depth = max_depth;
                }
            }
        }
    }

    //if we split and have objects, add them to children
    if (objects != nullptr && children != nullptr) {
        for (int i = 0; i < object_count; i++) {
            auto object_bounds = objects[i]->get_bounds();
            for (int j = 0; j < 2; j++) {
                if (children[j]->bounds.intersects(object_bounds)) {
                    children[j]->add_object(objects[i]);
                }
            }
        }
        //delete objects;
        objects = nullptr;
        //we don't set object_count to 0 because we use it to check if we have objects
    }

    //if we did split, build children
    if(children != nullptr){
#pragma omp parallel for
        for(int i=0; i<2; i++){
            children[i]->build(depth+1);
        }
    }
}

void BVHNode::shake(){
    if(children != nullptr){
//#pragma omp parallel for
        for(int i=0; i<2; i++){
            if(children[i] == nullptr){
                continue;
            }
            if(children[i]->object_count == 0){
                delete children[i];
                children[i] = nullptr;
            } else {
                children[i]->shake();
            }
        }
        recalculate_bounds();
    }
}


void BVHNode::get_intersecting(Ray ray, std::unordered_set<SceneObject*>& _objects) {
    if (!bounds.intersects_fast(ray)) {
        return;
    }

    //if i'm a leaf and i intersect, add my objects to _objects
    if (children == nullptr) {
        _objects.insert(objects, objects + object_count);
        return;
    }

    //if i'm not a leaf, check my children
    for (int i = 0; i < 2; i++) {
        if (children[i] == nullptr) {
            continue;
        }
        if (children[i]->bounds.intersects_fast(ray)) {
            children[i]->get_intersecting(ray, _objects);
        }
    }
}


/*
 SceneObject** BVHNode::get_intersecting(Ray ray, int& n) {
    if (!bounds.intersects(ray)) {
        n = -1;
        return nullptr;
    }

    std::vector<SceneObject*> _objects;

    if (depth == max_depth) {
        for (int i = 0; i < object_count; i++) {
            // check if object is already in the list
            bool found = std::any_of(_objects.begin(), _objects.end(), [&](SceneObject* obj) { return obj == objects[i]; });

            if (!found) {
                _objects.push_back(objects[i]);
            }
        }
    } else {
        for (int i = 0; i < 8; i++) {
            int count = 0;
            SceneObject** a = children[i].get_intersecting(ray, count);
            if(a == nullptr){
                continue;
            }
            for (int j = 0; j < count; j++) {
                // check if object is already in the list
                bool found = std::any_of(_objects.begin(), _objects.end(), [&](SceneObject* obj) { return obj == a[j]; });

                if (!found) {
                    _objects.push_back(a[j]);
                }
            }

            // Clean up memory if children[i].get_intersecting() allocates memory
            delete[] a;
        }
    }

    n = _objects.extents();

    // Create a dynamically allocated array to hold the pointers
    SceneObject** array = new SceneObject*[_objects.extents()];
    std::copy(_objects.begin(), _objects.end(), array);

    return array;
}
*/
