//
// Created by molar on 27/08/2023.
//

#include "Image.h"
Image::Image(int _width, int _height) {
    width = _width;
    height = _height;

    pixels = new Vector3*[width];
    for(int x=0; x<width; x++){
        pixels[x] = new Vector3[height];
    }
}

void Image::set_pixel(int x, int y, Vector3 color) {
    if(x < 0 || x >= width || y < 0 || y >= height){
        std::cout << "x: " << x << " y: " << y << std::endl;
        throw std::out_of_range("Index out of range.");
    }
    this->pixels[x][y] = color;
}

void Image::fill(Vector3 color){
    for(int x=0; x<width; x++){
        for(int y=0; y<height; y++){
            pixels[x][y] = color;
        }
    }
}

void Image::save(const std::string& path, EImageFormat format){
    std::ofstream f(path.c_str());
    if(!f.good()){
        throw std::runtime_error("Could not open file.");
    }

    switch(format){
        case EImageFormat::PPM:{
            save_ppm(f);
            break;
        }
    }
}

void Image::save(const std::string& path, EImageFormat format, int samples_per_pixel){
    std::ofstream f(path.c_str());
    if(!f.good()){
        throw std::runtime_error("Could not open file.");
    }

    switch(format){
        case EImageFormat::PPM:{
            save_ppm(f, samples_per_pixel);
            break;
        }
    }
}

void Image::save_ppm(std::ofstream& f){
    f << "P3\n";
    f << width << " ";
    f << height;
    f << "\n255\n";

    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            f << pixels[x][y].as_color() << "\n";
        }
    }
}

void Image::save_ppm(std::ofstream& f, int samples_per_pixel){
    f << "P3\n";
    f << width << " ";
    f << height;
    f << "\n255\n";

    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            f << (pixels[x][y]/samples_per_pixel).as_color_linear() << "\n";
        }
    }
}

void Image::test_pattern() {
    // simple cross at the center
    for(int x=0; x<width; x++){
        for(int y=0; y<height; y++){
            if(x == width / 2 || y == height / 2){
                pixels[x][y] = Vector3(1, 1, 1);
            } else {
                pixels[x][y] = Vector3(0, 0, 0);
            }
        }
    }
}

void Image::load(const std::string& path){
    cimg_library::CImg<unsigned char> image(path.c_str());
    width = image.width();
    height = image.height();

    pixels = new Vector3*[width];
    for(int x=0; x<width; x++){
        pixels[x] = new Vector3[height];
    }

    for(int x=0; x<width; x++){
        for(int y=0; y<height; y++){
            pixels[x][y] = Vector3(image(x, y, 0), image(x, y, 1), image(x, y, 2)) / 255.0;
        }
    }
}

Vector3 Image::get_pixel(int x, int y) {
#ifdef DEBUG
    if(x < 0 || x >= width || y < 0 || y >= height){
        std::cout << "x: " << x << " y: " << y << std::endl;
        throw std::out_of_range("Index out of range.");
    }
#endif
    return pixels[x][y];
}

void Image::normalize(int samples_per_pixel) {
    for(int x=0; x<width; x++){
        for(int y=0; y<height; y++){
            pixels[x][y] = pixels[x][y] / samples_per_pixel;
        }
    }
}

void test_image(){
    auto* img = new Image(512, 256);
    img->fill(Vector3{0, 0, 0});
    img->set_pixel(0, 0, Vector3{1, 0, 0});
    img->set_pixel(255, 255, Vector3{0, 1, 0});
    img->set_pixel(255, 0, Vector3{0, 0, 1});
    img->set_pixel(0, 255, Vector3{1, 1, 0});
    img->save("./test.ppm", EImageFormat::PPM);
}

void test_pattern(){
    //do everysize from 10x10 to 200x200, 10 increments
    for(int x=0; x<20; x++){
        for(int y=0; y<20; y++){
            auto* img = new Image(10 * (x + 1), 10 * (y + 1));
            img->test_pattern();
            std::string path = "./test_pattern.ppm";
            img->save(path, EImageFormat::PPM);
            _sleep(100);
        }
    }
}
