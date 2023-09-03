//
// Created by airon on 03/09/2023.
//

#ifndef RAYTRACER_VOLUMETESTSCENE_H
#define RAYTRACER_VOLUMETESTSCENE_H


#include "../Scene.h"

class VolumeTestScene : public Scene{
public:
    VolumeTestScene() : Scene("VolumeTestScene") {}
    void init_scene() override;
};


#endif //RAYTRACER_VOLUMETESTSCENE_H
