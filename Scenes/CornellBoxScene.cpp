//
// Created by airon on 03/09/2023.
//

#include "CornellBoxScene.h"
#include "../Objects/AAB.h"
#include "../materials/Lambertian.h"

void CornellBoxScene::init_scene() {
    //walls
    auto* left_wall = new AAB(10, 10, 0.1, Vector3(-5, 0, 0));
    left_wall->material = new Lambertian(Vector3(1, 0, 0));
    add_object(left_wall);

    auto* right_wall = new AAB(10, 10, 0.1, Vector3(5, 0, 0));
    right_wall->material = new Lambertian(Vector3(0, 1, 0));
    add_object(right_wall);

    auto* back_wall = new AAB(0.1, 10, 10, Vector3(0, 0, -5));
    back_wall->material = new Lambertian(Vector3(1, 1, 1));
    add_object(back_wall);

    auto* front_wall = new AAB(0.1, 10, 10, Vector3(0, 0, 5));
    front_wall->material = new Lambertian(Vector3(1, 1, 1));
    add_object(front_wall);

    auto* top_wall = new AAB(10, 0.1, 10, Vector3(0, 5, 0));
    top_wall->material = new Lambertian(Vector3(1, 1, 1));
    add_object(top_wall);

    auto* bottom_wall = new AAB(10, 0.1, 10, Vector3(0, -5, 0));
    bottom_wall->material = new Lambertian(Vector3(1, 1, 1));
    add_object(bottom_wall);

    //light
    auto* light = new AAB(1, 0.1, 1, Vector3(0, 4.9, 0));
    light->material = new Lambertian(Vector3(1, 1, 1));
    add_object(light);

    //environment function
    set_environment([](Ray ray){
        auto unit_direction = ray.Direction.unit_vector();
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3 (0.5, 0.7, 1.0);
    });
}
