//
// Created by molar on 30/08/2023.
//

#include "Texture.h"

Vector3 Texture::sample(double u, double v) {
    int x = (int)(u * image->width);
    int y = (int)((1 - v) * image->height - 0.001);

    //wrap texture
    while(x >= image->width)
        x -= image->width;

    while(y >= image->height)
        y -= image->height;

    while(x < 0)
        x += image->width;

    while(y < 0)
        y += image->height;

    return image->get_pixel(x, y);


}
