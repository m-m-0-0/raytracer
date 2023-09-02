//
// Created by molar on 28/08/2023.
//

#include "Camera.h"
#include "Interval.h"
#include "Image.h"
#include "Vector3.h"

Vector3 Camera::shoot_ray(const Ray &ray, int max_bounces) {
    static int count = 0;
    count++;

    if(max_bounces <= 0){
        return Vector3(0, 0, 0);
    }

    Hit closest_hit;

    std::unordered_set <SceneObject*> objects_set;
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

Image* Camera::render() {
    auto* img = new Image(width, height);

    setup_values();
    scene->build_bvh();

    //render
    #pragma omp parallel for default(none) shared(img, std::cout)
    for(int y=0; y<height; y++){
        if(y % 10 == 0){
            std::cout << "Rendering line " << y << std::endl;
        }
        #pragma omp parallel for default(none) shared(img, y, std::cout)
        for(int x=0; x<width; x++){
            try {
                for(int s=0; s<samples; s++){
                    auto ray = get_random_ray(x, y);
                    auto color = shoot_ray(ray, max_bounces);
                    img->set_pixel(x, y, img->get_pixel(x, y) + color);
                }
            } catch(std::exception& e){
                std::cout << e.what() << std::endl;
            }
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
