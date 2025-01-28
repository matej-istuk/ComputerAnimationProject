//
// Created by babinsinko on 27/03/24.
//

#include "model/Mesh.h"
#include <utility>
#include <vector>
#include "glm/mat4x4.hpp"
#include <algorithm>

std::pair<glm::vec3, glm::vec3> Mesh::getBoundingBox() {
    glm::vec3 min = glm::vec3(vertices.front().x, vertices.front().y, vertices.front().z);
    glm::vec3 max = glm::vec3(vertices.front().x, vertices.front().y, vertices.front().z);

    for (glm::vec4 vert: this->vertices) {
        if (vert.x < min.x) {
            min.x = vert.x;
        }
        if (vert.y < min.y) {
            min.y = vert.y;
        }
        if (vert.z < min.z) {
            min.z = vert.z;
        }
        if (vert.x > max.x) {
            max.x = vert.x;
        }
        if (vert.y > max.y) {
            max.y = vert.y;
        }
        if (vert.z > max.z) {
            max.z = vert.z;
        }
    }
    return {min, max};
}

void Mesh::applyTransform(glm::mat4 transform) {
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = transform * vertices[i];
    }
    //TODO change normals if necessary
}

Mesh::Mesh(std::vector<glm::vec4> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvCoords,
           std::vector<int> indices):
        Mesh(std::move(vertices), std::move(normals), std::move(uvCoords), std::move(indices), false) {};


Mesh::Mesh(std::vector<glm::vec4> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvCoords,
           std::vector<int> indices, bool normalizeMesh):
        vertices(std::move(vertices)),
        normals(std::move(normals)),
        uvCoords(std::move(uvCoords)),
        indices(std::move(indices)),
        VAO(0),
        VBO(std::vector<GLuint>(3)),
        EBO(0){
    if (normalizeMesh) {
        normalize();
    }
}

void Mesh::normalize() {
    std::pair<glm::vec3, glm::vec3> boundingBox = this->getBoundingBox();
    glm::mat4 transpose = glm::mat4(1.0);
    glm::mat4 scale = glm::mat4(1.0);
    transpose[3] -= glm::vec4((boundingBox.first.x + boundingBox.second.x) / 2,
                              (boundingBox.first.y + boundingBox.second.y) / 2,
                              (boundingBox.first.z + boundingBox.second.z) / 2,
                              0);
    float scalingFactor = std::min({2 / (boundingBox.second.x - boundingBox.first.x),
                                   2 / (boundingBox.second.y - boundingBox.first.y),
                                   2 / (boundingBox.second.z - boundingBox.first.z)});
    scale[0][0] = scalingFactor;
    scale[1][1] = scalingFactor;
    scale[2][2] = scalingFactor;
    
    this->applyTransform(transpose);
    this->applyTransform(scale);
}

void Mesh::accept(RenderVisitor& renderVisitor) {
    renderVisitor.visitMesh(*this);
}

GLuint Mesh::getVAO() const {
    return VAO;
}

void Mesh::setVAO(GLuint vao) {
    VAO = vao;
}

const std::vector<GLuint> &Mesh::getVBO() const {
    return VBO;
}

void Mesh::setVBO(const std::vector<GLuint> &vbo) {
    VBO = vbo;
}

GLuint Mesh::getEbo() const {
    return EBO;
}

void Mesh::setEBO(GLuint ebo) {
    EBO = ebo;
}

const std::vector<glm::vec4> &Mesh::getVertices() const {
    return vertices;
}

const std::vector<glm::vec3> &Mesh::getNormals() const {
    return normals;
}

const std::vector<glm::vec2> &Mesh::getUvCoords() const {
    return uvCoords;
}

const std::vector<int> &Mesh::getIndices() const {
    return indices;
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(VBO.size(), VBO.data());
    glDeleteVertexArrays(1, &EBO);
}
