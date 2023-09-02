//
// Created by molar on 28/08/2023.
//

#ifndef RAYTRACER_UTILS_H
#define RAYTRACER_UTILS_H

#include "Vector3.h"
#include <cstdlib>

inline double random_double(){
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    return min + (max-min) * random_double();
}

inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

inline double degrees_to_radians(double degrees){
    return degrees * M_PI / 180.0;
}

#endif //RAYTRACER_UTILS_H
