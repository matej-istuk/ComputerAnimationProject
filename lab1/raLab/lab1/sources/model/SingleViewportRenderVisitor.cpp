//
// Created by babinsinko on 3/30/24.
//

#include "model/SingleViewportRenderVisitor.h"
#include "model/Scene.h"
#include "model/Object.h"
#include "model/Mesh.h"
#include "glm/detail/type_mat4x4.hpp"
#include "model/CoordinateSystem.h"
#include "model/Curve.h"

void SingleViewportRenderVisitor::visitScene(Scene& scene) {
    camera = scene.getCamera();
    light = scene.getLight();
}

void SingleViewportRenderVisitor::visitObject(Object& object) {
    SingleViewportRenderVisitor::activeObject = &object;
    object.getShader().use();
    object.getShader().setUniform("view_matrix", camera->getViewMatrix());
    object.getShader().setUniform("perspective_matrix", camera->getPerspectiveMatrix());
    object.getShader().setUniform("model_matrix", object.getModelMatrix());
    if (object.hasMaterial() && light != nullptr) {
        polygonMode = GL_FILL;
        object.getShader().setUniform("eye_pos", camera->getPosition());
        object.getShader().setUniform("light_pos",  light->getPosition());
        object.getShader().setUniform("light_ambient", light->getAmbientIntensity());
        object.getShader().setUniform("light_diffuse", light->getDiffuseIntensity());
        object.getShader().setUniform("light_specular", light->getSpecularIntensity());
        object.getShader().setUniform("material_ambient", object.getMaterial()->getAmbientCoefficient());
        object.getShader().setUniform("material_diffuse", object.getMaterial()->getDiffuseCoefficient());
        object.getShader().setUniform("material_specular", object.getMaterial()->getSpecularCoefficient());
        object.getShader().setUniform("material_reflective", object.getMaterial()->getReflectiveExponent());
    } else {
        polygonMode = GL_LINE;
    }
    if (object.hasTexture()) {
        glBindTexture(GL_TEXTURE_2D, object.getTexture()->getTextureId());
    }
}

void SingleViewportRenderVisitor::visitMesh(Mesh& mesh) {
    glBindVertexArray(mesh.getVAO());
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
        glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


SingleViewportRenderVisitor::SingleViewportRenderVisitor(): camera(), activeObject() {}

void SingleViewportRenderVisitor::visitCoordinateSystem(CoordinateSystem &coordinateSystem) {
    glBindVertexArray(coordinateSystem.getVAO());
        glDrawArrays(GL_LINES, 0, coordinateSystem.getIndices().size());
    glBindVertexArray(0);
}

void SingleViewportRenderVisitor::visitCurve(Curve &curve) {
    if (curve.isChanged()){
        glBindVertexArray(curve.getVAO());
            glBindBuffer(GL_ARRAY_BUFFER, curve.getVBO()[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * curve.getLinePoints().size(), &curve.getLinePoints()[0], GL_STATIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(float), (void*)0);

            glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        curve.setChanged(false);
    }
    activeObject->getShader().setUniform("color", curve.getColor());
    glBindVertexArray(curve.getVAO());
        glDrawArrays(GL_LINE_STRIP, 0, curve.getLinePoints().size());
    glBindVertexArray(0);
}


