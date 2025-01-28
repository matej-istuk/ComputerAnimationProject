//
// Created by babinsinko on 5/19/24.
//

#include "model/Curve.h"

GLuint Curve::getVAO() const {
    return VAO;
}

void Curve::setVAO(GLuint vao) {
    VAO = vao;
}

const std::vector<GLuint> &Curve::getVBO() const {
    return VBO;
}

void Curve::setVBO(const std::vector<GLuint> &vbo) {
    VBO = vbo;
}

void Curve::accept(RenderVisitor &renderVisitor) {
    renderVisitor.visitCurve(*this);
}

Curve::Curve():
        color(glm::vec3(0)),
        changed(false),
        VAO(0),
        VBO(std::vector<GLuint>(3)){}

const glm::vec3 &Curve::getColor() const {
    return color;
}

void Curve::setColor(const glm::vec3 &color) {
    Curve::color = color;
}

bool Curve::isChanged() const {
    return changed;
}

void Curve::setChanged(bool changed) {
    Curve::changed = changed;
}

