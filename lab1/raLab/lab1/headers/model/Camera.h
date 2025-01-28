//
// Created by babinsinko on 4/1/24.
//

#ifndef IRGLAB_CAMERA_H
#define IRGLAB_CAMERA_H


#include "model/Transformable.h"
#include "Frustum.h"

class Camera: public Transformable {
private:
    Frustum frustum;
    glm::vec4 lookAtPoint;
public:
    Camera();

    glm::mat4 getViewMatrix();
    glm::mat4 getPerspectiveMatrix();

    void setLookAtPoint(const glm::vec4 &lookAtPoint);
};


#endif //IRGLAB_CAMERA_H
