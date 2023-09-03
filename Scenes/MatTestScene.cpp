//
// Created by airon on 03/09/2023.
//

#include "MatTestScene.h"
#include "../Objects/AAB.h"
#include "../materials/Lambertian.h"
#include "../Objects/Sphere.h"
#include "../materials/Dielectric.h"
#include "../materials/Metallic.h"

void MatTestScene::init_scene() {
    auto* Ground = new AAB(10, 0.1, 10, Vector3(0, -1, 0));
    Ground->material = new Lambertian(Vector3(0.5, 0.5, 0.5));
    add_object(Ground);

    auto* sphere = new Sphere(1);
    sphere->Transform.set_position(Vector3(-2, .5, -1));
    sphere->material = new Dielectric(Vector3(1, 0,0), 1.5);
    add_object(sphere);

    auto* sphere2 = new Sphere(1);
    sphere2->Transform.set_position(Vector3(0, .5, -1));
    sphere2->material = new Metallic(Vector3(1,1,0), 0.1);
    add_object(sphere2);

    auto* sphere3 = new Sphere(1);
    sphere3->Transform.set_position(Vector3(2, .5, -1));
    sphere3->material = new Lambertian(Vector3(0,0,1));
    add_object(sphere3);

    //environment function
    set_environment([](Ray ray){
        auto unit_direction = ray.Direction.unit_vector();
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3(0.5, 0.7, 1.0);
    });
}