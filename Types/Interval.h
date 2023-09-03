//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_INTERVAL_H
#define RAYTRACER_INTERVAL_H


#include <cmath>

struct Interval {
    double min;
    double max;

    Interval(double min, double max){
        this->min = min;
        this->max = max;
    }

    Interval(){
        this->min = -INFINITY;
        this->max = INFINITY;
    }

    bool contains(double value){
        return min <= value && value <= max;
    }

    bool contains(Interval interval){
        return min <= interval.min && interval.max <= max;
    }

    bool surrounds(double value){
        return min < value && value < max;
    }

    bool surrounds(Interval interval){
        return min < interval.min && interval.max < max;
    }

    double clamp(double value){
        return std::fmin(std::fmax(value, min), max);
    }

    static const Interval empty, universe;
};

#endif //RAYTRACER_INTERVAL_H
