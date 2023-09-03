//
// Created by molar on 31/08/2023.
//

#include "Bounds.h"
#include "Interval.h"
#include "Ray.h"

void Bounds::set_min_max(Vector3 min, Vector3 max) {
    extents = (max - min) / 2;
    center = min + extents;
}

void Bounds::encapsulate(Vector3 point){
    Vector3 min = center - extents;
    Vector3 max = center + extents;

    Vector3 new_min = Vector3(
            fmin(min.x, point.x),
            fmin(min.y, point.y),
            fmin(min.z, point.z)
    );

    Vector3 new_max = Vector3(
            fmax(max.x, point.x),
            fmax(max.y, point.y),
            fmax(max.z, point.z)
    );

    set_min_max(new_min, new_max);
}

void Bounds::encapsulate(Bounds bounds) {
    encapsulate(bounds.center - bounds.extents);
    encapsulate(bounds.center + bounds.extents);
}

bool Bounds::contains(Bounds bounds) {
    Vector3 min = center - extents ;
    Vector3 max = center + extents ;

    Vector3 bounds_min = bounds.center - bounds.extents ;
    Vector3 bounds_max = bounds.center + bounds.extents ;

    return min.x <= bounds_min.x && bounds_max.x <= max.x &&
           min.y <= bounds_min.y && bounds_max.y <= max.y &&
           min.z <= bounds_min.z && bounds_max.z <= max.z;
}

bool Bounds::contains(Vector3 point) {
    Vector3 min = center - extents ;
    Vector3 max = center + extents ;

    return min.x <= point.x && point.x <= max.x &&
           min.y <= point.y && point.y <= max.y &&
           min.z <= point.z && point.z <= max.z;
}

bool Bounds::intersects(Bounds bounds){
    Vector3 min = center - extents;
    Vector3 max = center + extents;

    Vector3 bounds_min = bounds.center - bounds.extents;
    Vector3 bounds_max = bounds.center + bounds.extents;

    return min.x <= bounds_max.x && bounds_min.x <= max.x &&
           min.y <= bounds_max.y && bounds_min.y <= max.y &&
           min.z <= bounds_max.z && bounds_min.z <= max.z;
}

bool Bounds::intersects(Ray ray){
    auto tmin = (center.x - extents.x - ray.Origin.x) / ray.Direction.x;
    auto tmax = (center.x + extents.x - ray.Origin.x) / ray.Direction.x;

    if(tmin > tmax){
        std::swap(tmin, tmax);
    }

    auto tymin = (center.y - extents.y - ray.Origin.y) / ray.Direction.y;
    auto tymax = (center.y + extents.y - ray.Origin.y) / ray.Direction.y;

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

    auto tzmin = (center.z - extents.z - ray.Origin.z) / ray.Direction.z;
    auto tzmax = (center.z + extents.z - ray.Origin.z) / ray.Direction.z;

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

    auto dist = Interval(0, INFINITY);

    if(!dist.surrounds(tmin)){
        tmin = tmax;
        if(!dist.surrounds(tmin)){
            return false;
        }
    }

    return true;
}