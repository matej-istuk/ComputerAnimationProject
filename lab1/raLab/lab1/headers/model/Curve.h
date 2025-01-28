//
// Created by babinsinko on 5/19/24.
//

#ifndef IRGLAB_CURVE_H
#define IRGLAB_CURVE_H


#include <vector>
#include "Renderable.h"
#include "glm/vec4.hpp"
#include "glad/glad.h"
#include "glm/vec3.hpp"

class Curve: public Renderable{
private:
    glm::vec3 color;
    bool changed;
    GLuint VAO{};
    std::vector<GLuint> VBO;
public:
    Curve();

    virtual std::vector<glm::vec4> getLinePoints() = 0;
    virtual glm::vec4 value(float t) = 0;
    void accept(RenderVisitor &renderVisitor) override;

    const glm::vec3 &getColor() const;

    void setColor(const glm::vec3 &color);

    GLuint getVAO() const;
    void setVAO(GLuint vao);
    const std::vector<GLuint> &getVBO() const;
    void setVBO(const std::vector<GLuint> &vbo);

    bool isChanged() const;

    void setChanged(bool changed);
};


#endif //IRGLAB_CURVE_H
