//
// Created by molar on 27/08/2023.
//

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H


#include "Vector3.h"
#include "iostream"
#include "Cimg.h"
#include <fstream>
//jpeglib-turbo
#include <jpeglib.h>

enum EImageFormat{
    PPM
};

class Image {
    Vector3** pixels;

public:
    int width;
    int height;

    Image(int width, int height);

    void set_pixel(int x, int y, Vector3 color);

    void fill(Vector3 color);

    void save(const std::string& path, EImageFormat format);

    void save_ppm(std::ofstream &f);

    void test_pattern();

    Vector3 get_pixel(int x, int y);

    void save_ppm(std::ofstream &f, int samples_per_pixel);

    void save(const std::string &path, EImageFormat format, int samples_per_pixel);

    void load(const std::string& path);

    void normalize(int samples_per_pixel);
};


#endif //RAYTRACER_IMAGE_H
