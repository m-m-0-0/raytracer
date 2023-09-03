//
// Created by molar on 29/08/2023.
//

#ifndef RAYTRACER_TRANSFORM_H
#define RAYTRACER_TRANSFORM_H


#include "Vector3.h"

class Transform {
private:
    Vector3 _forward;
    Vector3 _right;
    Vector3 _up;

    Vector3 _position;
    Vector3 _rotation;
    Vector3 _scale;

    void update_vectors(){
        _forward = Vector3(
                cos(degrees_to_radians(_rotation.x)) * cos(degrees_to_radians(_rotation.y)),
                sin(degrees_to_radians(_rotation.y)),
                sin(degrees_to_radians(_rotation.x)) * cos(degrees_to_radians(_rotation.y))
        ).normalized();
        _right = _forward.cross(Vector3(0, 1, 0)).normalized();
        _up = _right.cross(_forward).normalized();
    }

public:

    Vector3 forward(){
        return _forward;
    }

    Vector3 right(){
        return _right;
    }

    Vector3 up(){
        return _up;
    }

    Vector3 position(){
        return _position;
    }

    Vector3 rotation(){
        return _rotation;
    }

    Vector3 scale(){
        return _scale;
    }

    void set_position(Vector3 position){
        this->_position = position;
        update_vectors();
    }

    void set_rotation(Vector3 rotation){
        this->_rotation = rotation;
        update_vectors();
    }

    void set_scale(Vector3 scale){
        this->_scale = scale;
        update_vectors();
    }

    Transform(){
        _position = Vector3();
        _rotation = Vector3();
        _scale = Vector3(1, 1, 1);
        update_vectors();
    }

    Transform(Vector3 position, Vector3 rotation, Vector3 scale){
        this->_position = position;
        this->_rotation = rotation;
        this->_scale = scale;
        update_vectors();
    }

    Transform(Vector3 position){
        this->_position = position;
        this->_rotation = Vector3();
        this->_scale = Vector3(1, 1, 1);
        update_vectors();
    }
};


#endif //RAYTRACER_TRANSFORM_H