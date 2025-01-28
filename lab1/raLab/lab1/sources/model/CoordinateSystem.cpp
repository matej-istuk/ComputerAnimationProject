//
// Created by babinsinko on 4/1/24.
//

#include "model/CoordinateSystem.h"

void CoordinateSystem::accept(RenderVisitor &renderVisitor) {
    renderVisitor.visitCoordinateSystem(*this);
}

CoordinateSystem::~CoordinateSystem() {
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(VBO.size(), VBO.data());
    glDeleteVertexArrays(1, &EBO);
}

CoordinateSystem::CoordinateSystem(): VAO(0), EBO(0) {
    vertices.emplace_back(100, 0, 0, 1);
    vertices.emplace_back(-100, 0, 0, 1);
    vertices.emplace_back(0, 100, 0, 1);
    vertices.emplace_back(0, -100, 0, 1);
    vertices.emplace_back(0, 0, 100, 1);
    vertices.emplace_back(0, 0, -100, 1);
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);
}

const std::vector<glm::vec4> &CoordinateSystem::getVertices() const {
    return vertices;
}

const std::vector<int> &CoordinateSystem::getIndices() const {
    return indices;
}

GLuint CoordinateSystem::getVAO() const {
    return VAO;
}

const std::vector<GLuint> &CoordinateSystem::getVBO() const {
    return VBO;
}

GLuint CoordinateSystem::getEBO() const {
    return EBO;
}

void CoordinateSystem::setVAO(GLuint vao) {
    VAO = vao;
}

void CoordinateSystem::setVBO(const std::vector<GLuint> &vbo) {
    VBO = vbo;
}

void CoordinateSystem::setEBO(GLuint ebo) {
    EBO = ebo;
}
