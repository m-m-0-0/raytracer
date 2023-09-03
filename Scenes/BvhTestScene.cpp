//
// Created by airon on 03/09/2023.
//

#include "BvhTestScene.h"
#include "../Objects/Sphere.h"
#include "../materials/Lambertian.h"
#include "../materials/Metallic.h"
#include "../materials/Dielectric.h"

void BvhTestScene::init_scene() {
    //grid of balls in front of the camera
    int n_spheres = 100;
    for(int i=0; i<n_spheres; i++){
        auto* sphere = new Sphere(random_double(0.02, 0.04));
        //on the ground, in front of the camera
        double x = (i % 10) / 10.0;
        double y = (i / 10) / 10.0;
        sphere->Transform.set_position(Vector3(x, y, 1));
        double mat_choice = random_double();
        if(mat_choice < 0.33){
            sphere->material = new Lambertian(Vector3::random(0,1));
        } else if(mat_choice < 0.66){
            sphere->material = new Metallic(Vector3::random(0,1), random_double(0, 0.5));
        } else {
            sphere->material = new Dielectric(Vector3(1, 1, 1), random_double(1.1, 2));
        }

        add_object(sphere);
    }

    //environment function
    set_environment([](Ray ray){
        auto unit_direction = ray.Direction.unit_vector();
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3(0.5, 0.7, 1.0);
    });
}