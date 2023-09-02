//
// Created by molar on 28/08/2023.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H


#include "Vector3.h"
#include "Ray.h"
#include "utils.h"
#include "Scene.h"
#include <iostream>

class Image;

class Camera {
private:
    double viewport_width;
    double viewport_height;
    double focal_length = 1;
    Vector3 viewport_top_left;
    Vector3 viewport_u;
    Vector3 viewport_v;
    Vector3 pixel_top_left;
    Vector3 pixel_delta_u;
    Vector3 pixel_delta_v;

    Vector3 u, v, w;

public:
    Transform Transform;
    Scene* scene;
    int width;
    int height;
    double vertical_fov = 90;
    int max_bounces = 10;

    int samples = 10;

    void look_at(Vector3 position){
        focal_length = (Transform.position() - position).length();

        w = (Transform.position() - position).unit_vector();
        u = Transform.up().cross(w).unit_vector();
        v = w.cross(u);
    }

    void setup_values(){
        auto theta = degrees_to_radians(vertical_fov);
        auto h = tan(theta/2);
        viewport_height = 2.0 * h * focal_length;
        viewport_width = ((double)width/height) * viewport_height;

        viewport_u = viewport_width * u;
        viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u  / width;
        pixel_delta_v = viewport_v / height;

        viewport_top_left = Transform.position() - (focal_length * w) - (viewport_u / 2) - (viewport_v / 2);
        pixel_top_left = viewport_top_left + (pixel_delta_u / 2) + (pixel_delta_v / 2);
    }

    Camera(){
        Transform.set_position(Vector3(0, 0, 0));
        width = 640;
        height = 480;
        focal_length = 1;
    }

    Camera(Vector3 center){
        Transform.set_position(center);
        width = 640;
        height = 480;
        focal_length = 1;
    }

    Camera(Vector3 center, int width, int height){
        Transform.set_position(center);
        this->width = width;
        this->height = height;
    }

    Camera(Vector3 center, int width, int height, double focal_length){
        Transform.set_position(center);
        this->width = width;
        this->height = height;
        this->focal_length = focal_length;
    }

    void set_scene(Scene* _scene){
        this->scene = _scene;
    }

    Scene* get_scene(){
        return scene;
    }

    Ray get_random_ray(int x, int y);

    Image* render();

    Vector3 pixel_sample_offset();
    Vector3 shoot_ray(const Ray &ray, int max_bounces);

};


#endif //RAYTRACER_CAMERA_H
