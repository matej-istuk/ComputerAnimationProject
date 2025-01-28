//
// Created by babinsinko on 5/21/24.
//

#include "model/Light.h"

Light::Light(const glm::vec3 &ambientIntensity, const glm::vec3 &diffuseIntensity, const glm::vec3 &specularIntensity)
        : ambientIntensity(ambientIntensity), diffuseIntensity(diffuseIntensity),
          specularIntensity(specularIntensity) {}

Light::Light(): Light(1) {}

const glm::vec3 &Light::getAmbientIntensity() const {
    return ambientIntensity;
}

void Light::setAmbientIntensity(const glm::vec3 &ambientIntensity) {
    Light::ambientIntensity = ambientIntensity;
}

const glm::vec3 &Light::getDiffuseIntensity() const {
    return diffuseIntensity;
}

void Light::setDiffuseIntensity(const glm::vec3 &diffuseIntensity) {
    Light::diffuseIntensity = diffuseIntensity;
}

const glm::vec3 &Light::getSpecularIntensity() const {
    return specularIntensity;
}

void Light::setSpecularIntensity(const glm::vec3 &specularIntensity) {
    Light::specularIntensity = specularIntensity;
}

Light::Light(float strength): ambientIntensity(glm::vec3(strength)), diffuseIntensity(glm::vec3(strength)),
specularIntensity(glm::vec3(strength)) {

}
