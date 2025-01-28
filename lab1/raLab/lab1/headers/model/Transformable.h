//
// Created by babinsinko on 27/03/24.
//

#ifndef IRGLAB_TRANSFORMABLE_H
#define IRGLAB_TRANSFORMABLE_H


#include "glm/vec4.hpp"
#include "glm/fwd.hpp"

class Transformable {
private:
    glm::vec4 front;
    glm::vec4 up;
    glm::vec4 right;
    glm::vec4 position;
    glm::vec4 scale;
public:
    Transformable();

    Transformable(const glm::vec4 &front, const glm::vec4 &up, const glm::vec4 &right, const glm::vec4 &position,
                  const glm::vec4 &scale);

    glm::mat4 getModelMatrix();
    void localRotate(glm::vec3 angle);
    void globalRotate(glm::vec3 angle);
    void localMove(glm::vec3 move);
    void globalMove(glm::vec3 move);
    void doScale(glm::vec3 scaleChange);

    void setPosition(const glm::vec4 &position);

    void setFront(const glm::vec4 &front);

    void setUp(const glm::vec4 &up);

    void setRight(const glm::vec4 &right);

    const glm::vec4 &getFront() const;

    const glm::vec4 &getUp() const;

    const glm::vec4 &getRight() const;

    const glm::vec4 &getPosition() const;

    const glm::vec4 &getScale() const;
};


#endif //IRGLAB_TRANSFORMABLE_H
