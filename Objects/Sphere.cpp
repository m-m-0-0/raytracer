//
// Created by molar on 29/08/2023.
//

#include "Sphere.h"
#include "../Types/Interval.h"

bool Sphere::intersect(Ray ray, Interval dist, Hit &hit) {
    Vector3 oc = ray.Origin - Transform.position();
    auto a = ray.Direction.length_squared();
    auto half_b = oc.dot(ray.Direction);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if(discriminant < 0){
        return false;
    }
    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    if(!dist.surrounds(root)){
        root = (-half_b + sqrtd) / a;
        if(!dist.surrounds(root)){
            return false;
        }
    }
    hit.object = this;

    hit.hit = true;
    hit.t = root;
    hit.position = ray.at(hit.t);
    hit.normal = get_normal(ray, hit.t);
    hit.set_face_normal(ray, hit.normal);
    hit.material = material;
    return true;
}

Vector3 Sphere::get_normal(Ray ray, double t) {
    return (ray.at(t) - Transform.position()) / radius;
}

Bounds * Sphere::get_bounds() {
    if(bounds == nullptr){
        bounds = new Bounds(Transform.position(), Vector3(radius, radius, radius));
    }
    return bounds;
}
