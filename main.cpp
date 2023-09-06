#include <iostream>
#include "Image.h"
#include "Camera.h"
#include "utils.h"
#include "Scene.h"
#include "Types/Window.h"
#include <Windows.h>
#include <chrono>
#include <vector>

#include "oidn/include/OpenImageDenoise/oidn.hpp"
#include "Scenes/ManyBallsScene.h"
#include "Scenes/VolumeTestScene.h"
#include "Scenes/MatTestScene.h"
#include "Scenes/BvhTestScene.h"
#include "Scenes/CornellBoxScene.h"
#include "Scenes/TestScene2.h"
#include "Denoiser.h"

//TODO: properly fix this by using inherited classes
std::vector<std::unique_ptr<Scene>> scenes;

void Vector3_unit_test();

void test_bvh_depth(Scene* scene, Camera* camera, Window* window, int depth_max);

void prepare_scenes(){
    scenes.push_back(std::make_unique<ManyBallsScene>(4000));
    scenes.push_back(std::make_unique<VolumeTestScene>());
    scenes.push_back(std::make_unique<MatTestScene>());
    scenes.push_back(std::make_unique<BvhTestScene>());
    scenes.push_back(std::make_unique<CornellBoxScene>());
    scenes.push_back(std::make_unique<TestScene2>());
}

int main(int argc, char* argv[]) {
    //test_image();
    //test_pattern();

    //TODO: see where this actually belongs in chronological order
    prepare_scenes();

    Vector3_unit_test();

    SDL_Init(SDL_INIT_VIDEO);

    auto start = std::chrono::high_resolution_clock::now();

    //auto aspect_ratio = 16.0 /
    //cli is raytracer.exe width height samples bounces
    auto image_width = argc > 1 ? atoi(argv[1]) : 1920;
    auto image_height = argc > 2 ? atoi(argv[2]) : 1080;
    //image_height = (image_height < 1) ? 1 : image_height;
    auto aspect_ratio = image_width / (double)image_height;


    auto* camera = new Camera{Vector3(1,.8,3)};
    camera->width = image_width;
    camera->height = image_height;
    camera->samples = argc > 3 ? atoi(argv[3]) : 8;
    camera->max_bounces = argc > 4 ? atoi(argv[4]) : 4;
    camera->tiles_size = argc > 5 ? atoi(argv[5]) : 64;

    //create scene
    //Scene* scene = test_scene_2();
    Scene* scene = scenes[5].get();
    //build the scene
    scene->init_scene();
    camera->set_scene(scene);

    //camera.look_at(camera.Transform.forward() + camera.Transform.position());
    scene->set_bvh_max_depth(6);
    scene->build_bvh();
    auto center = scene->get_bounds();
    camera->look_at(center.center);
    std::cout << "focal length: " << camera->focal_length << std::endl;


    //test_bvh_depth(scene, camera, nullptr, 11);

    Window* window = new Window(image_width, image_height);

    //test_bvh_depth(scene, camera, window, 12);
    Image* image = new Image(image_width, image_height);
    window->set_image(image);
    window->set_samples_per_pixel(camera->samples);
    window->update_continuously_threaded(30);

    camera->render(image);
    image->normalize(camera->samples);

    image->save("./tmp.ppm", EImageFormat::PPM);

    //denoise image
    std::cout << "Denoising..." << std::endl;

    Denoiser denoiser = Denoiser();
    Image* denoised = denoiser.denoise(image);

    window->set_samples_per_pixel(1);
    window->set_image(denoised);


    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "Rendered in " << duration << "ms" << std::endl;
    std::cout << "Total rays: " << camera->width * camera->height * camera->samples << std::endl;
    std::cout << "Speed: " << (double)(camera->width * camera->height * camera->samples) / duration / 1000.0 << " Mrays/s" << std::endl;
    std::cout << "Resolution: " << camera->width << "x" << camera->height << std::endl;
    std::cout << "Samples: " << camera->samples << std::endl;
    std::cout << "Bounces: " << camera->max_bounces << std::endl;
    std::cout << "objects: " << scene->get_object_count() << std::endl;
    std::cout << "BVH depth: " << scene->get_bvh_max_depth() << std::endl;

    //animation
    int frames = 50;
    for(int i=0; i<frames; i++){
        std::cout << "Frame " << i << std::endl;
        camera->Transform.set_position(camera->Transform.position() + Vector3(0, 0, -0.1));
        camera->look_at(scene->get_bounds().center);

        Image* image = new Image(image_width, image_height);
        window->set_image(image);
        window->set_samples_per_pixel(camera->samples);
        camera->render(image);

        image->normalize(camera->samples);
        window->set_samples_per_pixel(1);

        Image* denoised = denoiser.denoise(image);
        window->set_image(denoised);

        denoised->save("./tmp" + std::to_string(i) + ".ppm", EImageFormat::PPM);
    }

    while(true){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                window->close();
                exit(0);
            }
        }
    }
}

void test_bvh_depth(Scene* scene, Camera* camera, Window* window, int depth_max){
    //bvh times array
    auto bvh_times = new long int[depth_max];

    //testing bvh depth
    for(int i=1; i<depth_max; i++){
        scene->set_bvh_max_depth(i);
        scene->build_bvh();
        std::cout << "BVH depth: " << i << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        Image* image = camera->render();
        auto end = std::chrono::high_resolution_clock::now();
        bvh_times[i] = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        image->normalize(camera->samples);
        image->save("./tmp" + std::to_string(i) + ".ppm", EImageFormat::PPM);
    }

    for(int i=1; i<depth_max; i++){
        //ms and MRays per second
        std::cout << "BVH depth: " << i << ", time: " << bvh_times[i] << "ms" << "(" << (camera->width * camera->height * camera->samples) / (bvh_times[i] / 1000.0) / 1000000.0 << " MRays/s)" << std::endl;
    }
}

void Vector3_unit_test(){
    Vector3 v1(1, 2, 3);
    Vector3 v2(4, 5, 6);

    std::cout << "v1: " << v1 << std::endl;
    std::cout << "v2: " << v2 << std::endl;
    std::cout << "v1 + v2: " << v1 + v2 << std::endl;
    std::cout << "v1 - v2: " << v1 - v2 << std::endl;
    std::cout << "v1 * v2: " << v1 * v2 << std::endl;
    std::cout << "v1 / v2: " << v1 / v2 << std::endl;
    std::cout << "v1 * 2: " << v1 * 2 << std::endl;
    std::cout << "v1 / 2: " << v1 / 2 << std::endl;
}