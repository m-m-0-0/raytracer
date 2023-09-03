//
// Created by molar on 31/08/2023.
//

#ifndef RAYTRACER_MODEL_H
#define RAYTRACER_MODEL_H


#include "../Types/Vector3.h"

class Model {
public:
    Vector3* vertices;
    Vector3* normals;
    Vector3* uvs;
    int* indices;
    int* tris;
    int num_vertices;
    int num_normals;
    int num_uvs;
    int num_indices;
    int num_tris;

    void load_obj(const std::string& path);
};


#endif //RAYTRACER_MODEL_H
