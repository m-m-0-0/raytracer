//
// Created by molar on 31/08/2023.
//

#ifndef RAYTRACER_BOUNDS_H
#define RAYTRACER_BOUNDS_H


#include "Vector3.h"
#include "Ray.h"

struct Bounds {
    Vector3 center;
    Vector3 extents;

    Bounds(Vector3 center, Vector3 size){
        this->center = center;
        this->extents = size;
    }

    Bounds(){
        this->center = Vector3();
        this->extents = Vector3();
    }

    void encapsulate(Bounds bounds);
    void encapsulate(Vector3 point);

    bool contains(Bounds bounds);

    bool contains(Vector3 point);

    bool intersects(Bounds bounds);

    bool intersects(Ray ray);

    void set_min_max(Vector3 min, Vector3 max);

    inline bool Bounds::intersects_fast(Ray ray){
        double tmin = 0.0;
        double tmax = INFINITY;

        Vector3 max = center + extents;
        Vector3 min = center - extents;

        for (int d = 0; d < 3; ++d) {
            double t1 = (min[d] - ray.Origin[d]) * ray.DirectionInverse[d];
            double t2 = (max[d] - ray.Origin[d]) * ray.DirectionInverse[d];

            tmin = std::max(tmin, std::min(t1, t2));
            tmax = std::min(tmax, std::max(t1, t2));
        }

        return tmin < tmax;
    }
};


#endif //RAYTRACER_BOUNDS_H
