#include <iostream>
#include "Image.h"
#include "Camera.h"
#include "utils.h"
#include "Scene.h"
#include "Objects/Sphere.h"
#include "Objects/AAB.h"
#include "materials/Lambertian.h"
#include "materials/Metallic.h"
#include "materials/Dielectric.h"
#include "Texture.h"
#include "materials/VolumeTest.h"
#include "Window.h"
#include <Windows.h>
#include <chrono>

Scene* many_balls_scene(int n_balls);
Scene* mat_test_scene();
Scene* volume_test_scene();
Scene* bvh_test_scene();

int main(int argc, char* argv[]) {
    //test_image();
    //test_pattern();

    SDL_Init(SDL_INIT_VIDEO);

    auto start = std::chrono::high_resolution_clock::now();

    auto aspect_ratio = 16.0 / 9.0;
    auto image_width = 800;

    auto image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    Camera camera{Vector3(0,1,-10)};
    camera.width = image_width;
    camera.height = image_height;
    camera.samples = 50;
    camera.max_bounces = 10;

    //create scene
    Scene* scene = many_balls_scene(1000);
    scene->set_bvh_max_depth(8);
    camera.set_scene(scene);

    camera.look_at(camera.Transform.forward());

    //camera.render("./tmp.ppm");

    /*
    camera.Transform.set_position(Vector3(-2.5, 0, 1));
    for(int i=0; i<50; i++){
        camera.render("./frame" + std::to_string(i) + ".ppm");
        camera.Transform.set_position(camera.Transform.position() + Vector3(0.1, 0, 0));
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "Rendered in " << duration << "ms" << std::endl;
    */

    Window* window = new Window(image_width, image_height);

    auto image = camera.render();
    image->normalize(camera.samples);
    window->update(image);
    image->save("./tmp.ppm", EImageFormat::PPM);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "Rendered in " << duration << "ms" << std::endl;

    //wait for window to close (sdl)
    while(true){
        window->handle_events();
        Sleep(100);
    }
    window->close();
    return 0;
}

Scene* many_balls_scene(int n_balls){
    //create scene
    Scene* scene = new Scene();

    auto* Ground = new AAB(10, 0.1, 10, Vector3(0, -1, 0));
    Ground->material = new Lambertian(Vector3(0.5, 0.5, 0.5));
    //scene->add_object(Ground);

    int n_spheres = n_balls;
    for(int i=0; i<n_spheres; i++){
        auto* sphere = new Sphere(random_double(0.05, 0.1));
        //on the ground, in front of the camera
        sphere->Transform.set_position(Vector3(random_double(-4, 4), random_double(0,5), random_double(-10, 0)));
        double mat_choice = random_double();
        if(mat_choice < 0.33){
            sphere->material = new Lambertian(Vector3::random(0,1));
        } else if(mat_choice < 0.66){
            sphere->material = new Metallic(Vector3::random(0,1), random_double(0, 0.5));
        } else {
            sphere->material = new Dielectric(Vector3(1, 1, 1), random_double(1.1, 2));
        }

        scene->add_object(sphere);
    }

    //environment function
    scene->set_environment([](Ray ray){
        auto unit_direction = ray.Direction.unit_vector();
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3(0.5, 0.7, 1.0);
    });

    return scene;
}

Scene* volume_test_scene(){
    Scene* scene = new Scene();

    auto* Ground = new AAB(1000, 0.1, 1000, Vector3(0, -1, 0));
    Ground->material = new Lambertian(Vector3(0.5, 0.5, 0.5));
    scene->add_object(Ground);

    auto* sphere = new Sphere(1);
    sphere->Transform.set_position(Vector3(0, .5, -1));
    sphere->material = new VolumeTest(Vector3(1, 1, 1), .5, 0.1);
    scene->add_object(sphere);

    //environment function
    scene->set_environment([](Ray ray){
        auto unit_direction = ray.Direction.unit_vector();
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3(0.5, 0.7, 1.0);
    });

    return scene;
}

Scene* mat_test_scene(){
    Scene* scene = new Scene();

    auto* Ground = new AAB(10, 0.1, 10, Vector3(0, -1, 0));
    Ground->material = new Lambertian(Vector3(0.5, 0.5, 0.5));
    scene->add_object(Ground);

    auto* sphere = new Sphere(1);
    sphere->Transform.set_position(Vector3(-2, .5, -1));
    sphere->material = new Dielectric(Vector3(1, 0,0), 1.5);
    scene->add_object(sphere);

    auto* sphere2 = new Sphere(1);
    sphere2->Transform.set_position(Vector3(0, .5, -1));
    sphere2->material = new Metallic(Vector3(1,1,0), 0.1);
    scene->add_object(sphere2);

    auto* sphere3 = new Sphere(1);
    sphere3->Transform.set_position(Vector3(2, .5, -1));
    sphere3->material = new Lambertian(Vector3(0,0,1));
    scene->add_object(sphere3);

    //environment function
    scene->set_environment([](Ray ray){
        auto unit_direction = ray.Direction.unit_vector();
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3(0.5, 0.7, 1.0);
    });

    return scene;
}

Scene* bvh_test_scene(){
    Scene* scene = new Scene();

    //grid of balls in front of the camera
    int n_spheres = 100;
    for(int i=0; i<n_spheres; i++){
        auto* sphere = new Sphere(random_double(0.02, 0.04));
        //on the ground, in front of the camera
        double x = (i % 10) / 10.0;
        double y = (i / 10) / 10.0;
        sphere->Transform.set_position(Vector3(x, y, -2));
        double mat_choice = random_double();
        if(mat_choice < 0.8){
            sphere->material = new Lambertian(Vector3::random(0,1));
        } else if(mat_choice < 0.95){
            sphere->material = new Metallic(Vector3::random(0,1), random_double(0, 0.5));
        } else {
            sphere->material = new Dielectric(Vector3(1, 1, 1), random_double(1.1, 2));
        }

        scene->add_object(sphere);
    }

    //environment function
    scene->set_environment([](Ray ray){
        auto unit_direction = ray.Direction.unit_vector();
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3(0.5, 0.7, 1.0);
    });

    return scene;
}