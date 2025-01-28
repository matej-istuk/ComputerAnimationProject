//
// Created by babinsinko on 27/03/24.
//

#include "model/Transformable.h"
#include "glm/detail/type_mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transformable::getModelMatrix() {
    glm::mat4 rotPosMat = glm::mat4(right, up, front, position);
    rotPosMat[0][3] = 0;
    rotPosMat[1][3] = 0;
    rotPosMat[2][3] = 0;
    rotPosMat[3][3] = 1;
    return rotPosMat * glm::scale(glm::mat4(1.0f), glm::vec3(scale));
}

void Transformable::localRotate(glm::vec3 angle) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, angle.x, glm::vec3(this->right));
    trans = glm::rotate(trans, angle.y, glm::vec3(this->up));
    trans = glm::rotate(trans, angle.z, glm::vec3(this->front));
    this->front = this->front * trans;
    this->up = this->up * trans;
    this->right = this->right * trans;
}

void Transformable::globalRotate(glm::vec3 angle) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, angle.x, glm::vec3(1, 0, 0));
    trans = glm::rotate(trans, angle.y, glm::vec3(0, 1, 0));
    trans = glm::rotate(trans, angle.z, glm::vec3(0, 0, 1));
    this->front = trans * this->front;
    this->up = trans * this->up;
    this->right = trans * this->right;
    this->position = trans * position;
}


void Transformable::globalMove(glm::vec3 move) {
    this->position = glm::translate(glm::mat4(1.0f), move) * position;
}

void Transformable::localMove(glm::vec3 move) {
    glm::vec4 tempPosition = glm::inverse(getModelMatrix()) * position;
    tempPosition = glm::translate(glm::mat4(1.0f), move) * tempPosition;
    this->position = getModelMatrix() * tempPosition;
}

void Transformable::doScale(glm::vec3 scaleChange) {
    this->scale = glm::scale(glm::mat4(1.0f), scaleChange) * this->scale;
}

Transformable::Transformable():
        Transformable(
        glm::vec4(0, 0, 1 , 0),
        glm::vec4(0, 1, 0 , 0),
        glm::vec4(1, 0, 0 , 0),
        glm::vec4(0, 0, 0 , 1),
        glm::vec4(1, 1, 1 , 1)) {}

Transformable::Transformable(const glm::vec4 &front, const glm::vec4 &up, const glm::vec4 &right, const glm::vec4 &position,
                             const glm::vec4 &scale) : front(front), up(up), right(right), position(position), scale(scale) {}

const glm::vec4 &Transformable::getFront() const {
    return front;
}

const glm::vec4 &Transformable::getUp() const {
    return up;
}

const glm::vec4 &Transformable::getRight() const {
    return right;
}

const glm::vec4 &Transformable::getPosition() const {
    return position;
}

const glm::vec4 &Transformable::getScale() const {
    return scale;
}

void Transformable::setPosition(const glm::vec4 &position) {
    Transformable::position = position / position[3];
}

void Transformable::setFront(const glm::vec4 &front) {
    Transformable::front = front;
}

void Transformable::setUp(const glm::vec4 &up) {
    Transformable::up = up;
}

void Transformable::setRight(const glm::vec4 &right) {
    Transformable::right = right;
}
