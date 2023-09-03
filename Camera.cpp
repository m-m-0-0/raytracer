//
// Created by molar on 28/08/2023.
//

#include "Camera.h"
#include "Types/Interval.h"
#include "Image.h"
#include "Types/Vector3.h"
#include "Types/Window.h"
//chrono
#include <chrono>
Vector3 Camera::shoot_ray(const Ray &ray, int max_bounces) {
    static int count = 0;
    count++;

    if(max_bounces <= 0){
        return Vector3(0, 0, 0);
    }

    Hit closest_hit;

    std::unordered_set <SceneObject*> objects_set;
    objects_set.reserve(scene->get_object_count() / scene->get_bvh_max_depth());
    scene->get_root()->get_intersecting(ray, objects_set);

    for(SceneObject* object : objects_set){
        Hit hit;
        if(object->intersect(ray, Interval(0.001, INFINITY), hit)){
            if(hit.t < closest_hit.t){
                closest_hit = hit;
            }
        }
    }

    if(closest_hit.t < INFINITY && closest_hit.t > 0 && closest_hit.hit){
        Material* material = closest_hit.material;
        Ray scattered;
        Vector3 attenuation;
        if(material->scatter(ray, closest_hit, attenuation, scattered)){
            return attenuation * shoot_ray(scattered, max_bounces - 1);
        } else {
            return Vector3(0, 0, 0);
        }
    } else {
        return scene->sample_environment(ray);
    }
}

Vector3 Camera::shoot_ray_bvh(const Ray &ray, int max_bounces) {
    static int count = 0;
    count++;

    if(max_bounces <= 0){
        return Vector3(0, 0, 0);
    }

    Hit closest_hit;
    closest_hit.t = INFINITY;

    scene->get_root()->shoot_ray_bvh(ray, closest_hit);

    if(closest_hit.t < INFINITY && closest_hit.t > 0 && closest_hit.hit){
        Material* material = closest_hit.material;
        Ray scattered;
        Vector3 attenuation;
        if(material->scatter(ray, closest_hit, attenuation, scattered)){
            return attenuation * shoot_ray_bvh(scattered, max_bounces - 1);
        } else {
            return Vector3(0, 0, 0);
        }
    } else {
        return scene->sample_environment(ray);
    }
}

Hit Camera::shoot_shadow_rays(){
    return {};
}

Image* Camera::render(Window* window) {
    auto* img = new Image(width, height);

    scene->build_bvh();
    setup_values();

    int lines_done = 0;

    //render
    #pragma omp parallel for default(none) shared(img, std::cout, lines_done, window)
    for(int y=0; y<height; y++){
        std::chrono::high_resolution_clock::time_point start;
        if(y % 100 == 0){
            start = std::chrono::high_resolution_clock::now();
        }
        #pragma omp parallel for default(none) shared(img, y, std::cout)
        for(int x=0; x<width; x++){
            try {
                for(int s=0; s<samples; s++){
                    auto ray = get_random_ray(x, y);
                    auto color = shoot_ray_bvh(ray, max_bounces); //bvh
                    img->set_pixel(x, y, img->get_pixel(x, y) + color);
                }
            } catch(std::exception& e){
                std::cout << e.what() << std::endl;
            }
        }
        lines_done++;
        if(y % 10){
            window->redraw();
        }
        if(y % 100 == 0){
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
            std::cout << "Rendered line " << y << " in " << duration << "ms" << std::endl;
            //Mrays/s
            std::cout << "Speed: " << (double)(width * samples * lines_done) / duration / 1000.0 << " Mrays/s" << std::endl;
            //progress and estimated time
            std::cout << "Progress: " << (lines_done * 100.0) / height << "%, ";
            std::cout <<"Estimated time: " << (duration * (height - lines_done)) / 1000.0 / 10 << "s" << std::endl;

            //update image
            Image* scaled = new Image(width, height);
            for(int y=0; y<height; y++){
                for(int x=0; x<width; x++){
                    scaled->set_pixel(x, y, img->get_pixel(x, y));
                }
            }
            scaled->normalize(samples);
            window->update(scaled);
            delete scaled;
        }
    }

    return img;
}

Vector3 Camera::pixel_sample_offset(){
    double px = -0.5 + random_double();
    double py = -0.5 + random_double();
    return px * pixel_delta_u + py * pixel_delta_v;
}

Ray Camera::get_random_ray(int x, int y) {
    auto pixel_center = pixel_top_left + (x * pixel_delta_u) + (y * pixel_delta_v);
    auto pixel_sample = pixel_center + pixel_sample_offset();
    auto ray_direction = pixel_sample - Transform.position();
    return Ray(Transform.position(), ray_direction);
}
