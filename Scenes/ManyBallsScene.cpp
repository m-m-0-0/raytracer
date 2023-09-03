//
// Created by airon on 03/09/2023.
//
#include "ManyBallsScene.h"
#include "../Objects/AAB.h"
#include "../materials/Lambertian.h"
#include "../Objects/Sphere.h"
#include "../materials/Metallic.h"
#include "../materials/Dielectric.h"

ManyBallsScene::ManyBallsScene(int n_balls) {
    this->n_balls = n_balls;
}

void ManyBallsScene::init_scene() {
    //create scene
    auto* Ground = new AAB(10, 0.1, 10, Vector3(0, -1, 0));
    Ground->material = new Lambertian(Vector3(0.5, 0.5, 0.5));
    //scene->add_object(Ground);

    int n_spheres = n_balls;
    for(int i=0; i<n_spheres; i++){
        auto* sphere = new Sphere(random_double(0.05, 0.1));
        //on the ground, in front of the camera
        sphere->Transform.set_position(Vector3(random_double(-4, 4), random_double(-3,3), random_double(-4, 0)));
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
