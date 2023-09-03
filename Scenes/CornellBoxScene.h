//
// Created by airon on 03/09/2023.
//

#ifndef RAYTRACER_CORNELLBOXSCENE_H
#define RAYTRACER_CORNELLBOXSCENE_H


#include "../Scene.h"

class CornellBoxScene : public Scene {
public:
    CornellBoxScene() : Scene("CornellBoxScene") {};
    void init_scene() override;
};

#endif //RAYTRACER_CORNELLBOXSCENE_H
