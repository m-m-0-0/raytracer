//
// Created by airon on 03/09/2023.
//

#ifndef RAYTRACER_BVHTESTSCENE_H
#define RAYTRACER_BVHTESTSCENE_H


#include "../Scene.h"

class BvhTestScene : public Scene {
public:
    BvhTestScene() : Scene("BVHTestScene") {};
    void init_scene() override;
};


#endif //RAYTRACER_BVHTESTSCENE_H
