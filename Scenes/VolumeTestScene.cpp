//
// Created by airon on 03/09/2023.
//

#include "VolumeTestScene.h"
#include "../materials/VolumeTest.h"
#include "../Objects/Sphere.h"
#include "../materials/Lambertian.h"
#include "../Objects/AAB.h"

void VolumeTestScene::init_scene() {

    auto* Ground = new AAB(1000, 0.1, 1000, Vector3(0, -1, 0));
    Ground->material = new Lambertian(Vector3(0.5, 0.5, 0.5));
    add_object(Ground);

    auto* sphere = new Sphere(1);
    sphere->Transform.set_position(Vector3(0, .5, -1));
    sphere->material = new VolumeTest(Vector3(1, 1, 1), .5, 0.1);
    add_object(sphere);

    //environment function
    set_environment([](Ray ray){
        auto unit_direction = ray.Direction.unit_vector();
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3(0.5, 0.7, 1.0);
    });
}
