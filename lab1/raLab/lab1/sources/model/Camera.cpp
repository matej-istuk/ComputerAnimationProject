//
// Created by babinsinko on 4/1/24.
//

#include "model/Camera.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

Camera::Camera():
Transformable(),
frustum(),
lookAtPoint(glm::vec4(0, 0, 0, 1)) {}


glm::mat4 Camera::getPerspectiveMatrix() {
    return glm::frustum(frustum.getLeft(), frustum.getRight(), frustum.getBottom(), frustum.getTop(), frustum.getNear(), frustum.getFar());
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(glm::vec3(this->getPosition()), glm::vec3(lookAtPoint / lookAtPoint.w), glm::vec3(this->getUp()));
//    return glm::inverse(getModelMatrix());
}

void Camera::setLookAtPoint(const glm::vec4 &lookAtPoint) {
    Camera::lookAtPoint = lookAtPoint;
}
