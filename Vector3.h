//
// Created by molar on 27/08/2023.
//

#ifndef RAYTRACER_VECTOR3_H
#define RAYTRACER_VECTOR3_H

#include <SDL_pixels.h>
#include <SDL_stdinc.h>
#include "stdexcept"
#include "iostream"
#include "cmath"
#include "utils.h"
//for to_string
#include "string"

struct Vector3 {
    double x;
    double y;
    double z;

    Vector3(){
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    explicit Vector3(double value){
        this->x = value;
        this->y = value;
        this->z = value;
    }

    inline double operator[](int i) const{
        switch(i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                throw std::out_of_range("Vector3 only contains 3 elements.");
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    std::string as_color() const{
#ifdef DEBUG
        if(x > 1 || y > 1 || z > 1){
            throw std::out_of_range("Vector3 color values must be between 0 and 1.");
        }
#endif
        return std::to_string((int)(x * 255)) + " " + std::to_string((int)(y * 255)) + " " + std::to_string((int)(z * 255));
    }

    std::string as_color_linear() const{
        return std::to_string((int)(linear_to_gamma(x) * 255)) + " " + std::to_string((int)(linear_to_gamma(y) * 255)) + " " + std::to_string((int)(linear_to_gamma(z) * 255));
    }

    //to sdl color
    inline Uint32 to_sdl_color(SDL_PixelFormat* format) const{
        return SDL_MapRGB(format, (Uint8)(linear_to_gamma(x) * 255), (Uint8)(linear_to_gamma(y) * 255), (Uint8)(linear_to_gamma(z) * 255));
    }

    //addition
    inline Vector3 operator+(const Vector3& v) const{
        return {x + v.x, y + v.y, z + v.z};
    }

    //subtraction
    inline Vector3 operator-(const Vector3& v) const{
        return {x - v.x, y - v.y, z - v.z};
    }

    inline Vector3 operator-() const{
        return {-x, -y, -z};
    }

    //multiplication
    inline Vector3 operator*(const Vector3& v) const{
        return {x * v.x, y * v.y, z * v.z};
    }

    inline Vector3 operator*(const double& v) const{
        return {x * v, y * v, z * v};
    }

    friend inline Vector3 operator*(double v, const Vector3& vec) {
        return {vec.x * v, vec.y * v, vec.z * v};
    }

    //division
    inline Vector3 operator/(const Vector3& v) const{
        return {x / v.x, y / v.y, z / v.z};
    }

    inline Vector3 operator/(const double& v) const{
        return {x / v, y / v, z / v};
    }

    inline double dot(const Vector3& v) const{
        return x * v.x + y * v.y + z * v.z;
    }

    inline Vector3 cross(const Vector3& v) const{
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

    inline double length_squared() const{
        return x * x + y * y + z * z;
    }

    inline double length() const{
        return sqrt(length_squared());
    }

    inline Vector3 normalized() const{
        return *this / length();
    }

    inline Vector3 unit_vector() const{
        return *this / length();
    }

    inline Vector3 reflect(const Vector3& normal) const{
        return *this - normal * (2 * this->dot(normal));
    }

    inline Vector3 refract(const Vector3& normal, double etai_over_etat) const{
        auto cos_theta = fmin(-this->dot(normal), 1.0);
        Vector3 r_out_perp = (*this + normal * cos_theta) * etai_over_etat;
        Vector3 r_out_parallel = normal * -sqrt(fabs(1.0 - r_out_perp.length_squared()));
        return r_out_perp + r_out_parallel;
    }

    inline bool near_zero() const{
        const auto s = 1e-8;
        if(fabs(x) < s && fabs(y) < s && fabs(z) < s){
            return true;
        }
        return false;
    }

    inline Vector3 lerp(const Vector3& v, double t) const{
        return *this * (1 - t) + v * t;
    }

    static inline Vector3 random(){
        return {random_double(), random_double(), random_double()};
    }

    static inline Vector3 random(double min, double max){
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }

    static inline Vector3 random_in_unit_sphere(){
        while(true){
            auto p = random(-1, 1);
            if(p.length_squared() >= 1) continue;
            return p;
        }
    }

    static inline Vector3 random_unit_vector(){
        return random_in_unit_sphere().unit_vector();
    }

    inline static Vector3 random_on_hemisphere(const Vector3& normal){
        auto on_unit_sphere = random_unit_vector();
        if(on_unit_sphere.dot(normal) > 0.0){
            return on_unit_sphere;
        } else {
            return -on_unit_sphere;
        }
    }

    inline static Vector3 lerp_gradient(Vector3* colors, int colors_num, double t){
        double color_sep = 1.0 / (colors_num - 1);
        int color_index = (int)(t / color_sep);
        double color_t = (t - (color_sep * color_index)) / color_sep;
        return colors[color_index].lerp(colors[color_index + 1], color_t);
    }
};


#endif //RAYTRACER_VECTOR3_H
