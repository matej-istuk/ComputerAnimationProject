//
// Created by babinsinko on 4/1/24.
//

#include "model/Frustum.h"
#include "glm/trigonometric.hpp"
Frustum::Frustum(float fov, float aspect, float near, float far) : fov(fov), aspect(aspect), near(near), far(far) {
    update();
}

Frustum::Frustum(): Frustum(3.14/4, 1.f, 0.5, 30.f) {}

void Frustum::update() {
    top = near * glm::tan(fov/2);
    bottom = -top;
    right = aspect * top;
    left = -right;
}

float Frustum::getFov() const {
    return fov;
}

void Frustum::setFov(float fov) {
    Frustum::fov = fov;
    update();
}

float Frustum::getAspect() const {
    return aspect;
}

void Frustum::setAspect(float aspect) {
    Frustum::aspect = aspect;
    update();
}

float Frustum::getNear() const {
    return near;
}

void Frustum::setNear(float near) {
    Frustum::near = near;
    update();
}

float Frustum::getFar() const {
    return far;
}

void Frustum::setFar(float far) {
    Frustum::far = far;
}

float Frustum::getLeft() const {
    return left;
}

float Frustum::getRight() const {
    return right;
}

float Frustum::getBottom() const {
    return bottom;
}

float Frustum::getTop() const {
    return top;
}
