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

//TODO: properly fix this by using inherited classes
std::vector<std::unique_ptr<Scene>> scenes;

void Vector3_unit_test();

void test_bvh_depth(Scene* scene, Camera* camera, Window* window, int depth_max);

void prepare_scenes(){
    scenes.push_back(std::make_unique<ManyBallsScene>(100));
    scenes.push_back(std::make_unique<VolumeTestScene>());
    scenes.push_back(std::make_unique<MatTestScene>());
    scenes.push_back(std::make_unique<BvhTestScene>());
    scenes.push_back(std::make_unique<CornellBoxScene>());
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


    auto* camera = new Camera{Vector3(0,1,3)};
    camera->width = image_width;
    camera->height = image_height;
    camera->samples = argc > 3 ? atoi(argv[3]) : 5;
    camera->max_bounces = argc > 4 ? atoi(argv[4]) : 10;

    //create scene
    //Scene* scene = test_scene_2();
    Scene* scene = scenes[4].get();
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

    auto image = camera->render(window);
    image->normalize(camera->samples);
    window->update(image);
    image->save("./tmp.ppm", EImageFormat::PPM);

    //denoise image
    std::cout << "Denoising..." << std::endl;

    oidn::DeviceRef device = oidn::newDevice();
    device.commit();
    oidn::FilterRef filter = device.newFilter("RT");
    oidn::BufferRef imageBuf = device.newBuffer(image_width * image_height * 3 * sizeof(float));

    auto image_data = image->get_pixels();
    float* image_data_float = new float[image_width * image_height * 3];
    for(int i=0; i<image_width * image_height; i++){
        image_data_float[i * 3] = image_data[i].x;
        image_data_float[i * 3 + 1] = image_data[i].y;
        image_data_float[i * 3 + 2] = image_data[i].z;
    }

    imageBuf.write(0, sizeof(float) * 3 * image_width * image_height, image_data_float);
    filter.setImage("color", imageBuf, oidn::Format::Float3, image_width, image_height);
    filter.setImage("output", imageBuf, oidn::Format::Float3, image_width, image_height);
    filter.set("hdr", false);
    filter.commit();

    float* colorPtr = (float*)imageBuf.getData();

    filter.execute();

    const char* errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << "Error: " << errorMessage << std::endl;

    for(int i=0; i<image_width * image_height; i++){
        image_data[i].x = colorPtr[i * 3];
        image_data[i].y = colorPtr[i * 3 + 1];
        image_data[i].z = colorPtr[i * 3 + 2];
    }

    image->set_pixels(image_data);

    window->update(image);
    image->save("./tmp_denoised.ppm", EImageFormat::PPM);

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

    //wait for window to close (sdl)
    while(true){
        window->handle_events();
        Sleep(100);
    }
    window->close();
    return 0;
}
/*

void test_bvh_depth(Scene* scene, Camera* camera, Window* window, int depth_max){
    //bvh times array
    auto bvh_times = new long int[depth_max];

    //testing bvh depth
    for(int i=1; i<depth_max; i++){
        scene->set_bvh_max_depth(i);
        scene->build_bvh();
        std::cout << "BVH depth: " << i << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        Image* image = camera->render(window);
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

Scene* test_scene_2(){
    auto scene = new Scene();

    AAB* ground = new AAB(10, 0.1, 10, Vector3(0, -0.05, 0));
    ground->material = new Lambertian(Vector3(0.8, 0.8, 0.8));
    scene->add_object(ground);

    int balls = 500;
    for(int i=0; i<balls; i++){
        auto* sphere = new Sphere(random_double(0.05, 0.1));
        //on the ground, in front of the camera
        sphere->Transform.set_position(Vector3(random_double(-2, 2), sphere->radius, random_double(-2,2)));
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
*/
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