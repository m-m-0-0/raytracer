//
// Created by molar on 27/08/2023.
//

#ifndef RAYTRACER_IMAGE_H
#define RAYTRACER_IMAGE_H


#include "Types/Vector3.h"
#include "iostream"
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

    Vector3* get_pixels(){
        Vector3* pixels = new Vector3[width * height];
        for(int x=0; x<width; x++){
            for(int y=0; y<height; y++){
                pixels[x + y * width] = this->pixels[x][y];
            }
        }
        return pixels;
    }

    void set_pixels(Vector3* pixels){
        for(int x=0; x<width; x++){
            for(int y=0; y<height; y++){
                this->pixels[x][y] = pixels[x + y * width];
            }
        }
    }

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

    Vector3 get_pixel(int x, int y, int samples_per_pixel);
};


#endif //RAYTRACER_IMAGE_H
