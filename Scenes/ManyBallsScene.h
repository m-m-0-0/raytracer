//
// Created by airon on 03/09/2023.
//

#ifndef RAYTRACER_MANYBALLSSCENE_H
#define RAYTRACER_MANYBALLSSCENE_H

#include "..\Scene.h"

class ManyBallsScene : public Scene {
public:
    int n_balls = 100;
    explicit ManyBallsScene(int n_balls);
    void init_scene() override;
};

#endif //RAYTRACER_MANYBALLSSCENE_H
