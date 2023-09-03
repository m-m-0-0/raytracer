//
// Created by airon on 03/09/2023.
//

#ifndef RAYTRACER_MATTESTSCENE_H
#define RAYTRACER_MATTESTSCENE_H


#include "../Scene.h"

class MatTestScene : public Scene {
public:
    explicit MatTestScene() : Scene("MatTestScene") {};
    void init_scene() override;
};


#endif //RAYTRACER_MATTESTSCENE_H
