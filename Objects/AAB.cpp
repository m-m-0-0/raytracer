//
// Created by molar on 29/08/2023.
//

#include "AAB.h"

bool AAB::intersect(Ray ray, Interval dist, Hit &hit) {
    auto tmin = (Transform.position().x - x/2 - ray.Origin.x) / ray.Direction.x;
    auto tmax = (Transform.position().x + x/2 - ray.Origin.x) / ray.Direction.x;

    if(tmin > tmax){
        std::swap(tmin, tmax);
    }

    auto tymin = (Transform.position().y - y/2 - ray.Origin.y) / ray.Direction.y;
    auto tymax = (Transform.position().y + y/2 - ray.Origin.y) / ray.Direction.y;

    if(tymin > tymax){
        std::swap(tymin, tymax);
    }

    if((tmin > tymax) || (tymin > tmax)){
        return false;
    }

    if(tymin > tmin){
        tmin = tymin;
    }

    if(tymax < tmax){
        tmax = tymax;
    }

    auto tzmin = (Transform.position().z - z/2 - ray.Origin.z) / ray.Direction.z;
    auto tzmax = (Transform.position().z + z/2 - ray.Origin.z) / ray.Direction.z;

    if(tzmin > tzmax){
        std::swap(tzmin, tzmax);
    }

    if((tmin > tzmax) || (tzmin > tmax)){
        return false;
    }

    if(tzmin > tmin){
        tmin = tzmin;
    }

    if(tzmax < tmax){
        tmax = tzmax;
    }

    if(!dist.surrounds(tmin)){
        tmin = tmax;
        if(!dist.surrounds(tmin)){
            return false;
        }
    }

    hit.object = this;
    hit.hit = true;
    hit.t = tmin;
    hit.position = ray.at(hit.t);
    hit.normal = get_normal(ray, hit.t);
    hit.set_face_normal(ray, hit.normal);
    hit.material = material;

    return true;
}

Vector3 AAB::get_normal(Ray ray, double t) {
    auto p = ray.at(t);
    //tolerance
    auto epsilon = 0.0001;
    if(abs(p.x - Transform.position().x - x/2) < epsilon){
        return Vector3(1, 0, 0);
    }
    if(abs(p.x - Transform.position().x + x/2) < epsilon){
        return Vector3(-1, 0, 0);
    }
    if(abs(p.y - Transform.position().y - y/2) < epsilon){
        return Vector3(0, 1, 0);
    }
    if(abs(p.y - Transform.position().y + y/2) < epsilon){
        return Vector3(0, -1, 0);
    }
    if(abs(p.z - Transform.position().z - z/2) < epsilon){
        return Vector3(0, 0, 1);
    }
    if(abs(p.z - Transform.position().z + z/2) < epsilon){
        return Vector3(0, 0, -1);
    }
    return Vector3(0, 0, 0);
}

Bounds * AAB::get_bounds() {
    if(bounds == nullptr){
        bounds = new Bounds(Transform.position(), Vector3(x/2, y/2, z/2));
    }
    return bounds;
}
