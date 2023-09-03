//
// Created by molar on 30/08/2023.
//

#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H


#include "Types/Vector3.h"
#include "Image.h"

class Texture {
public:
    Texture(){
        image = new Image(1, 1);
    }

    Texture(Image* image){
        this->image = image;
    }
    virtual Vector3 sample(double u, double v);

private:
    Image* image;
};


#endif //RAYTRACER_TEXTURE_H
