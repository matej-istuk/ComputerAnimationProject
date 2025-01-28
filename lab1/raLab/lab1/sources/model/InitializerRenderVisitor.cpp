//
// Created by babinsinko on 3/30/24.
//

#include "model/InitializerRenderVisitor.h"
#include "model/Scene.h"
#include "model/Object.h"
#include "model/Mesh.h"
#include "model/CoordinateSystem.h"
#include "model/Curve.h"
#include "glm/vec2.hpp"
#include "stb_image.h"

void InitializerRenderVisitor::visitScene(Scene& scene) {
}

void InitializerRenderVisitor::visitObject(Object& object) {
    if (object.hasTexture()) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        object.getTexture()->setTextureId(textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, object.getTexture()->getWidth(), object.getTexture()->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                         object.getTexture()->getData());
            glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(object.getTexture()->getData());
    }
}

void InitializerRenderVisitor::visitMesh(Mesh& mesh) {
    GLuint VAO;
    std::vector<GLuint> VBO = std::vector<GLuint>(3);
    GLuint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(3, VBO.data());
    glGenBuffers(1, &EBO);

    mesh.setVAO(VAO);
    mesh.setVBO(VBO);
    mesh.setEBO(EBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * mesh.getVertices().size(), &mesh.getVertices()[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.getNormals().size(), &mesh.getNormals()[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);


        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh.getUvCoords().size(), &mesh.getUvCoords()[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2* sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mesh.getIndices().size(), &mesh.getIndices()[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void InitializerRenderVisitor::visitCoordinateSystem(CoordinateSystem &coordinateSystem) {
    GLuint VAO;
    std::vector<GLuint> VBO = std::vector<GLuint>(1);
    GLuint EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, VBO.data());
    glGenBuffers(1, &EBO);

    coordinateSystem.setVAO(VAO);
    coordinateSystem.setVBO(VBO);
    coordinateSystem.setEBO(EBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * coordinateSystem.getVertices().size(), &coordinateSystem.getVertices()[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * coordinateSystem.getIndices().size(), &coordinateSystem.getIndices()[0], GL_STATIC_DRAW);
    glBindVertexArray(0);

}

void InitializerRenderVisitor::visitCurve(Curve &curve) {
    GLuint VAO;
    std::vector<GLuint> VBO = std::vector<GLuint>(3);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, VBO.data());

    curve.setVAO(VAO);
    curve.setVBO(VBO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * curve.getLinePoints().size(), &curve.getLinePoints()[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);
    glBindVertexArray(0);

}
