//
// Created by airon on 03/09/2023.
//

#ifndef RAYTRACER_TESTSCENE2_H
#define RAYTRACER_TESTSCENE2_H


#include "../Scene.h"

class TestScene2 : public Scene {
public:
    TestScene2() : Scene("TestScene2") {};
    void init_scene() override;
};

#endif //RAYTRACER_TESTSCENE2_H
