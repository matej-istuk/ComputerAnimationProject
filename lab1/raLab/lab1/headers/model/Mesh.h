//
// Created by babinsinko on 27/03/24.
//

#ifndef IRGLAB_MESH_H
#define IRGLAB_MESH_H

#include <vector>
#include <string>
#include "glm/vec4.hpp"
#include "glm/fwd.hpp"
#include "glad/glad.h"
#include "Renderable.h"
#include "RenderVisitor.h"

class Mesh: public Renderable{
private:

    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvCoords;
    std::vector<int> indices;
    GLuint VAO;
    std::vector<GLuint> VBO;
    GLuint EBO;

    void applyTransform(glm::mat4 transform);
    std::pair<glm::vec3, glm::vec3> getBoundingBox();
public:
    Mesh(std::vector<glm::vec4> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvCoords,
         std::vector<int> indices);
    Mesh(std::vector<glm::vec4> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvCoords,
         std::vector<int> indices, bool normalizeMesh);
    void normalize();

    void accept(RenderVisitor& renderVisitor) override;

    ~Mesh() override;

    GLuint getVAO() const;

    void setVAO(GLuint vao);

    const std::vector<GLuint> &getVBO() const;

    void setVBO(const std::vector<GLuint> &vbo);

    GLuint getEbo() const;

    void setEBO(GLuint ebo);

    const std::vector<glm::vec4> &getVertices() const;

    const std::vector<glm::vec3> &getNormals() const;

    const std::vector<glm::vec2> &getUvCoords() const;

    const std::vector<int> &getIndices() const;
};


#endif //IRGLAB_MESH_H
