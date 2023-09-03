//
// Created by molar on 31/08/2023.
//

#include "BVHNode.h"
#include <vector>
#include <algorithm>
#include <unordered_set>

void BVHNode::recalculate_bounds() {
    bounds = Bounds();
    //if leaf, recalculate bounds based on objects
    if (children == nullptr) {
        for (int i = 0; i < object_count; i++) {
            bounds.encapsulate(*objects[i]->get_bounds());
        }
    } else {
        for (int i = 0; i < 2; i++) {
            if (children[i] == nullptr) {
                continue;
            }
            bounds.encapsulate(children[i]->bounds);
        }
    }
}

void BVHNode::build(int _depth) {
    if (children == nullptr && objects != nullptr) {
        if (object_count > 0) {
            if (_depth < max_depth) {
                //we split!
                children = new BVHNode*[2]; //octree

                Vector3 extents = bounds.extents;
                Vector3 center = bounds.center;

                //split on longest axis

                Vector3 variance = {};
                for (int i = 0; i < object_count; i++) {
                    variance = variance + objects[i]->get_bounds()->center - center;
                }

                variance = variance / object_count;
                //split on max variance axis
                int axis = 0;
                if (fabs(variance.y) > fabs(variance.x)) {
                    axis = 1;
                }
                if (fabs(variance.z) > fabs(variance.y) && fabs(variance.z) > fabs(variance.x)) {
                    axis = 2;
                }

                Vector3 left = center;
                Vector3 right = center;
                Vector3 new_extents = extents;

                if (axis == 0) {
                    left.x -= extents.x / 2;
                    right.x += extents.x / 2;
                    new_extents.x *= 0.5;
                } else if (axis == 1) {
                    left.y -= extents.y / 2;
                    right.y += extents.y / 2;
                    new_extents.y *= 0.5;
                } else {
                    left.z -= extents.z / 2;
                    right.z += extents.z / 2;
                    new_extents.z *= 0.5;
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
        bool* to_add = new bool[object_count * 2];
        auto* obj_bounds = new Bounds[object_count];
        for (int i = 0; i < object_count; i++) {
            obj_bounds[i] = *objects[i]->get_bounds();
            to_add[i] = false;
            to_add[i + object_count] = false;
        }

        /*
        Bounds b01[4] = {obj_bounds[i%object_count], obj_bounds[(i+1)%object_count], obj_bounds[(i+2)%object_count], obj_bounds[(i+3)%object_count]};
        Bounds b02[4] = {children[0]->bounds, children[0]->bounds, children[0]->bounds, children[0]->bounds};
        bool* res0 = Bounds::intersects_simd_4(b01, b02);

        Bounds b12[4] = {children[1]->bounds, children[1]->bounds, children[1]->bounds, children[1]->bounds};
        bool* res1 = Bounds::intersects_simd_4(b01, b12);

        for(int j=0; j<4; j++){
            to_add[(i + j) % object_count] = res0[j];
            to_add[((i + j) % object_count) + object_count] = res1[j];
        }
            */

        for (int i = 0; i < object_count; i++) {
            if (children[0]->bounds.intersects_fast(obj_bounds[i])) {
                to_add[i] = true;
            }
            if (children[1]->bounds.intersects_fast(obj_bounds[i])) {
                to_add[i + object_count] = true;
            }
        }

        for (int i = 0; i < object_count; i++) {
            if (to_add[i]) {
                children[0]->add_object_no_bounds(objects[i]);
            }
            if (to_add[i + object_count]) {
                children[1]->add_object_no_bounds(objects[i]);
            }
        }

        //delete objects;
        objects = nullptr;
    }

    //if we did split, build children
    if(children != nullptr){
//#pragma omp parallel for
        for(int i=0; i<2; i++){
            if(children[i] == nullptr){
                continue;
            }
            children[i]->build(depth+1);
            children[i]->recalculate_bounds();
        }
    }
}

void BVHNode::shake(){
    if(children != nullptr) {
//#pragma omp parallel for
        for (int i = 0; i < 2; i++) {
            if (children[i] == nullptr) {
                continue;
            }
            if (children[i]->object_count == 0) {
                delete children[i];
                children[i] = nullptr;
            } else {
                children[i]->shake();
            }
        }

        if(children[0] == nullptr && children[1] == nullptr){
            delete[] children;
            children = nullptr;
            object_count = 0;
        }
    }
}


void BVHNode::get_intersecting(Ray ray, std::unordered_set<SceneObject*>& _objects) {
    if (depth == 0 && !bounds.intersects_fast(ray)) {
        return;
    }

    //if i'm a leaf and i intersect, add my objects to _objects
    if (children == nullptr) {
        if(objects == nullptr){
            return;
        }

        for (int i = 0; i < object_count; i++) {
            if(objects[i]->get_bounds()->intersects_fast(ray)){
                _objects.insert(objects[i]);
            }
        }
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

void BVHNode::shoot_ray_bvh(Ray ray, Hit &closest_hit){
    double t = -1;
    if(depth == 0 && (!bounds.intersects_fast(ray, t) || t > closest_hit.t)){
        return;
    }

    if(children == nullptr){
        if(objects == nullptr){
            return;
        }
        for(int i=0; i<object_count; i++){
            Hit hit;
            if (objects[i]->intersect(ray, Interval(0.001, INFINITY), hit)){
                if(hit.t < closest_hit.t){
                    closest_hit = hit;
                }
            }
        }
        return;
    }

    BVHNode* nodes[2] = { children[0], children[1] };
    double ts[2] = { INFINITY, INFINITY }; // initialize intersection times to infinity

    if (nodes[0] != nullptr) {
        nodes[0]->bounds.intersects_fast(ray, ts[0]);
    }
    if (nodes[1] != nullptr) {
        nodes[1]->bounds.intersects_fast(ray, ts[1]);
    }

    if (ts[1] < ts[0]) {
        std::swap(nodes[0], nodes[1]);
        std::swap(ts[0], ts[1]);
    }

    for (int i = 0; i < 2; ++i) {
        if (ts[i] < closest_hit.t) {
            nodes[i]->shoot_ray_bvh(ray, closest_hit);
        }
    }
}

void BVHNode::print_tree(int depth){
    for(int i=0; i<depth; i++){
        std::cout << "  ";
    }
    std::cout << "Node: " << bounds.center << " " << bounds.extents << " " << object_count << std::endl;
    if(children != nullptr){
        for(int i=0; i<2; i++){
            if(children[i] == nullptr){
                continue;
            }
            children[i]->print_tree(depth+1);
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
