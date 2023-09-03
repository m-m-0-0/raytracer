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

    inline bool intersects_fast_simd(Ray ray) const {
        double tmin = 0.0;
        double tmax = INFINITY;

        Vector3 max = center + extents;
        Vector3 min = center - extents;

        /*
        for (int d = 0; d < 3; ++d) {
            double t1 = (min[d] - ray.Origin[d]) * ray.DirectionInverse[d];
            double t2 = (max[d] - ray.Origin[d]) * ray.DirectionInverse[d];

            tmin = std::max(tmin, std::min(t1, t2));
            tmax = std::min(tmax, std::max(t1, t2));
        }*/
        //the above but with avx

        __m256d t1 = _mm256_mul_pd(_mm256_sub_pd(_mm256_set_pd(0, min.z, min.y, min.x), _mm256_set_pd(0, ray.Origin.z, ray.Origin.y, ray.Origin.x)), _mm256_set_pd(0, ray.DirectionInverse.z, ray.DirectionInverse.y, ray.DirectionInverse.x));
        __m256d t2 = _mm256_mul_pd(_mm256_sub_pd(_mm256_set_pd(0, max.z, max.y, max.x), _mm256_set_pd(0, ray.Origin.z, ray.Origin.y, ray.Origin.x)), _mm256_set_pd(0, ray.DirectionInverse.z, ray.DirectionInverse.y, ray.DirectionInverse.x));

        __m256d tminv = _mm256_max_pd(_mm256_min_pd(t1, t2), _mm256_set1_pd(tmin));
        __m256d tmaxv = _mm256_min_pd(_mm256_max_pd(t1, t2), _mm256_set1_pd(tmax));

        d256 tminres = {tminv};
        d256 tmaxres = {tmaxv};

        return tminres.d[0] < tmaxres.d[0] && tminres.d[1] < tmaxres.d[1] && tminres.d[2] < tmaxres.d[2];
    }

    inline bool intersects_fast(Ray ray) const {
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

    inline bool intersects_fast(Ray ray, double &t) const {
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

        if (tmin < tmax) {
            t = tmin;
            return true;
        } else {
            return false;
        }
    }

    inline bool intersects_fast_simd(Ray const &ray, double t){
        __m256d tmin = _mm256_set1_pd(0.0f);
        __m256d tmax = _mm256_set1_pd(INFINITY);

        __m256d _center = _mm256_set_pd(0, extents.z, extents.y, extents.x);
        __m256d _extents = _mm256_set_pd(0, center.z, center.y, center.x);
        __m256d min = _mm256_sub_pd(_center, _extents);
        __m256d max = _mm256_add_pd(_center, _extents);

        __m256d _ray_origin = _mm256_set_pd(0, ray.Origin.z, ray.Origin.y, ray.Origin.x);
        __m256d _ray_direction_inverse = _mm256_set_pd(0, ray.DirectionInverse.z, ray.DirectionInverse.y, ray.DirectionInverse.x);

        __m256d t1 = _mm256_mul_pd(_mm256_sub_pd(min, _ray_origin), _ray_direction_inverse);
        __m256d t2 = _mm256_mul_pd(_mm256_sub_pd(max, _ray_origin), _ray_direction_inverse);

        tmin = _mm256_max_pd(_mm256_min_pd(t1, t2), tmin);
        tmax = _mm256_min_pd(_mm256_max_pd(t1, t2), tmax);

        d256 tminres = {tmin};
        d256 tmaxres = {tmax};

        if(tminres.d[0] < tmaxres.d[0]){
            t = tminres.d[0];
            return true;
        }else{
            return false;
        }
    }

    inline bool intersects_fast(Bounds const &bounds){
        Vector3 min = center - extents;
        Vector3 max = center + extents;

        Vector3 bounds_min = bounds.center - bounds.extents;
        Vector3 bounds_max = bounds.center + bounds.extents;

        return min.x <= bounds_max.x && bounds_min.x <= max.x &&
            min.y <= bounds_max.y && bounds_min.y <= max.y &&
            min.z <= bounds_max.z && bounds_min.z <= max.z;
    }

    inline bool intersects_simd(Bounds const &b){
        /*    Vector3 min = center - extents;
        Vector3 max = center + extents;

        Vector3 bounds_min = bounds.center - bounds.extents;
        Vector3 bounds_max = bounds.center + bounds.extents;

        return min.x <= bounds_max.x && bounds_min.x <= max.x &&
            min.y <= bounds_max.y && bounds_min.y <= max.y &&
            min.z <= bounds_max.z && bounds_min.z <= max.z;*/

        __m256d _center = _mm256_set_pd(0, center.z, center.y, center.x);
        __m256d _extents = _mm256_set_pd(0, extents.z, extents.y, extents.x);
        __m256d min = _mm256_sub_pd(_center, _extents);
        __m256d max = _mm256_add_pd(_center, _extents);

        __m256d _b_center = _mm256_set_pd(0, b.center.z, b.center.y, b.center.x);
        __m256d _b_extents = _mm256_set_pd(0, b.extents.z, b.extents.y, b.extents.x);
        __m256d b_min = _mm256_sub_pd(_b_center, _b_extents);
        __m256d b_max = _mm256_add_pd(_b_center, _b_extents);

        d256 minres = {min};
        d256 maxres = {max};
        d256 b_minres = {b_min};
        d256 b_maxres = {b_max};

        return minres.d[0] <= b_maxres.d[0] && b_minres.d[0] <= maxres.d[0] &&
               minres.d[1] <= b_maxres.d[1] && b_minres.d[1] <= maxres.d[1] &&
               minres.d[2] <= b_maxres.d[2] && b_minres.d[2] <= maxres.d[2];
    }

    inline static bool* intersects_simd_4(Bounds* b1, Bounds* b2){
        __m256d cx1 = _mm256_set_pd(b1[3].center.x, b1[2].center.x, b1[1].center.x, b1[0].center.x);
        __m256d cy1 = _mm256_set_pd(b1[3].center.y, b1[2].center.y, b1[1].center.y, b1[0].center.y);
        __m256d cz1 = _mm256_set_pd(b1[3].center.z, b1[2].center.z, b1[1].center.z, b1[0].center.z);

        __m256d cx2 = _mm256_set_pd(b2[3].center.x, b2[2].center.x, b2[1].center.x, b2[0].center.x);
        __m256d cy2 = _mm256_set_pd(b2[3].center.y, b2[2].center.y, b2[1].center.y, b2[0].center.y);
        __m256d cz2 = _mm256_set_pd(b2[3].center.z, b2[2].center.z, b2[1].center.z, b2[0].center.z);

        __m256d ex1 = _mm256_set_pd(b1[3].extents.x, b1[2].extents.x, b1[1].extents.x, b1[0].extents.x);
        __m256d ey1 = _mm256_set_pd(b1[3].extents.y, b1[2].extents.y, b1[1].extents.y, b1[0].extents.y);
        __m256d ez1 = _mm256_set_pd(b1[3].extents.z, b1[2].extents.z, b1[1].extents.z, b1[0].extents.z);

        __m256d ex2 = _mm256_set_pd(b2[3].extents.x, b2[2].extents.x, b2[1].extents.x, b2[0].extents.x);
        __m256d ey2 = _mm256_set_pd(b2[3].extents.y, b2[2].extents.y, b2[1].extents.y, b2[0].extents.y);
        __m256d ez2 = _mm256_set_pd(b2[3].extents.z, b2[2].extents.z, b2[1].extents.z, b2[0].extents.z);

        __m256d minx1 = _mm256_sub_pd(cx1, ex1);
        __m256d miny1 = _mm256_sub_pd(cy1, ey1);
        __m256d minz1 = _mm256_sub_pd(cz1, ez1);

        __m256d maxx1 = _mm256_add_pd(cx1, ex1);
        __m256d maxy1 = _mm256_add_pd(cy1, ey1);
        __m256d maxz1 = _mm256_add_pd(cz1, ez1);

        __m256d minx2 = _mm256_sub_pd(cx2, ex2);
        __m256d miny2 = _mm256_sub_pd(cy2, ey2);
        __m256d minz2 = _mm256_sub_pd(cz2, ez2);

        __m256d maxx2 = _mm256_add_pd(cx2, ex2);
        __m256d maxy2 = _mm256_add_pd(cy2, ey2);
        __m256d maxz2 = _mm256_add_pd(cz2, ez2);

        //create unions
        d256 minx1res = {minx1};
        d256 miny1res = {miny1};
        d256 minz1res = {minz1};

        d256 maxx1res = {maxx1};
        d256 maxy1res = {maxy1};
        d256 maxz1res = {maxz1};

        d256 minx2res = {minx2};
        d256 miny2res = {miny2};
        d256 minz2res = {minz2};

        d256 maxx2res = {maxx2};
        d256 maxy2res = {maxy2};
        d256 maxz2res = {maxz2};

        //check if intersects
        bool* res = new bool[4];
        res[0] = minx1res.d[0] <= maxx2res.d[0] && miny1res.d[0] <= maxy2res.d[0] && minz1res.d[0] <= maxz2res.d[0] &&
                 minx2res.d[0] <= maxx1res.d[0] && miny2res.d[0] <= maxy1res.d[0] && minz2res.d[0] <= maxz1res.d[0];
        res[1] = minx1res.d[1] <= maxx2res.d[1] && miny1res.d[1] <= maxy2res.d[1] && minz1res.d[1] <= maxz2res.d[1] &&
                 minx2res.d[1] <= maxx1res.d[1] && miny2res.d[1] <= maxy1res.d[1] && minz2res.d[1] <= maxz1res.d[1];
        res[2] = minx1res.d[2] <= maxx2res.d[2] && miny1res.d[2] <= maxy2res.d[2] && minz1res.d[2] <= maxz2res.d[2] &&
                    minx2res.d[2] <= maxx1res.d[2] && miny2res.d[2] <= maxy1res.d[2] && minz2res.d[2] <= maxz1res.d[2];
        res[3] = minx1res.d[3] <= maxx2res.d[3] && miny1res.d[3] <= maxy2res.d[3] && minz1res.d[3] <= maxz2res.d[3] &&
                    minx2res.d[3] <= maxx1res.d[3] && miny2res.d[3] <= maxy1res.d[3] && minz2res.d[3] <= maxz1res.d[3];

        return res;
    }
};


#endif //RAYTRACER_BOUNDS_H
