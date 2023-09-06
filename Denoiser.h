//
// Created by molar on 04/09/2023.
//

#ifndef RAYTRACER_DENOISER_H
#define RAYTRACER_DENOISER_H

#include "Image.h"
#include "Types/Vector3.h"
#include "OpenImageDenoise/oidn.hpp"


class Denoiser {
private:
    oidn::DeviceRef device;

public:
    Denoiser(){
        device = oidn::newDevice();
        device.commit();
    }

    Image* denoise(Image* image){
        oidn::FilterRef filter = device.newFilter("RT");

        auto image_data = image->get_pixels();
        float* image_data_float = new float[image->width * image->height * 3];
        for(int i=0; i<image->width * image->height; i++){
            image_data_float[i * 3] = image_data[i].x;
            image_data_float[i * 3 + 1] = image_data[i].y;
            image_data_float[i * 3 + 2] = image_data[i].z;
        }

        oidn::BufferRef imageBuf = device.newBuffer(image->width * image->height * 3 * sizeof(float));
        oidn::BufferRef outputBuf = device.newBuffer(image->width * image->height * 3 * sizeof(float));

        imageBuf.write(0, sizeof(float) * 3 * image->width * image->height, image_data_float);
        filter.setImage("color", imageBuf, oidn::Format::Float3, image->width, image->height);
        filter.setImage("output", outputBuf, oidn::Format::Float3, image->width, image->height);
        filter.set("hdr", false);
        filter.commit();

        float* colorPtr = (float*)outputBuf.getData();

        filter.execute();

        const char* errorMessage;
        if (device.getError(errorMessage) != oidn::Error::None)
            throw std::runtime_error(errorMessage);


        for(int i=0; i<image->width * image->height; i++){
            image_data[i].x = colorPtr[i * 3];
            image_data[i].y = colorPtr[i * 3 + 1];
            image_data[i].z = colorPtr[i * 3 + 2];
        }

        Image* output = new Image(image->width, image->height);
        output->set_pixels(image_data);

        return output;
    }
};


#endif //RAYTRACER_DENOISER_H
