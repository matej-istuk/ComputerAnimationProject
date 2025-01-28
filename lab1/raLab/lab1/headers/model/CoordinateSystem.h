//
// Created by babinsinko on 4/1/24.
//

#ifndef IRGLAB_COORDINATESYSTEM_H
#define IRGLAB_COORDINATESYSTEM_H


#include <vector>
#include "Renderable.h"
#include "glm/vec4.hpp"
#include "glad/glad.h"
#include "glm/vec3.hpp"

class CoordinateSystem: public Renderable {
private:
    std::vector<glm::vec4> vertices;
    std::vector<int> indices;
    GLuint VAO;
    std::vector<GLuint> VBO;
    GLuint EBO;
public:
    explicit CoordinateSystem();

    void accept(RenderVisitor &renderVisitor) override;

    ~CoordinateSystem() override;

    const std::vector<glm::vec4> &getVertices() const;

    const glm::vec3 &getColor() const;

    const std::vector<int> &getIndices() const;

    GLuint getVAO() const;

    const std::vector<GLuint> &getVBO() const;

    GLuint getEBO() const;

    void setVAO(GLuint vao);

    void setVBO(const std::vector<GLuint> &vbo);

    void setEBO(GLuint ebo);
};


#endif //IRGLAB_COORDINATESYSTEM_H
