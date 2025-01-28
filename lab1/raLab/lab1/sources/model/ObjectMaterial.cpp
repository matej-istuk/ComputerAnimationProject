//
// Created by babinsinko on 5/21/24.
//

#include "../../headers/model/ObjectMaterial.h"

ObjectMaterial::ObjectMaterial(const glm::vec3 &ambientCoefficient, const glm::vec3 &diffuseCoefficient,
                               const glm::vec3 &specularCoefficient, float shininessExponent) : ambientCoefficient(
        ambientCoefficient), diffuseCoefficient(diffuseCoefficient), specularCoefficient(specularCoefficient),
                                                                                                reflectiveExponent(
                                                                                                        shininessExponent) {}

const glm::vec3 &ObjectMaterial::getAmbientCoefficient() const {
    return ambientCoefficient;
}

void ObjectMaterial::setAmbientCoefficient(const glm::vec3 &ambientCoefficient) {
    ObjectMaterial::ambientCoefficient = ambientCoefficient;
}

const glm::vec3 &ObjectMaterial::getDiffuseCoefficient() const {
    return diffuseCoefficient;
}

void ObjectMaterial::setDiffuseCoefficient(const glm::vec3 &diffuseCoefficient) {
    ObjectMaterial::diffuseCoefficient = diffuseCoefficient;
}

const glm::vec3 &ObjectMaterial::getSpecularCoefficient() const {
    return specularCoefficient;
}

void ObjectMaterial::setSpecularCoefficient(const glm::vec3 &specularCoefficient) {
    ObjectMaterial::specularCoefficient = specularCoefficient;
}

float ObjectMaterial::getReflectiveExponent() const {
    return reflectiveExponent;
}

void ObjectMaterial::setReflectiveExponent(float reflectiveExponent) {
    ObjectMaterial::reflectiveExponent = reflectiveExponent;
}

