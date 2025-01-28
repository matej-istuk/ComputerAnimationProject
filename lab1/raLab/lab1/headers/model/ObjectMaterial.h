//
// Created by babinsinko on 5/21/24.
//

#ifndef IRGLAB_OBJECTMATERIAL_H
#define IRGLAB_OBJECTMATERIAL_H


#include "glm/vec3.hpp"

class ObjectMaterial {
private:
    glm::vec3 ambientCoefficient;
    glm::vec3 diffuseCoefficient;
    glm::vec3 specularCoefficient;
    float reflectiveExponent;
public:
    ObjectMaterial(const glm::vec3 &ambientCoefficient, const glm::vec3 &diffuseCoefficient,
                   const glm::vec3 &specularCoefficient, float shininessExponent);

    const glm::vec3 &getAmbientCoefficient() const;

    void setAmbientCoefficient(const glm::vec3 &ambientCoefficient);

    const glm::vec3 &getDiffuseCoefficient() const;

    void setDiffuseCoefficient(const glm::vec3 &diffuseCoefficient);

    const glm::vec3 &getSpecularCoefficient() const;

    void setSpecularCoefficient(const glm::vec3 &specularCoefficient);

    float getReflectiveExponent() const;

    void setReflectiveExponent(float reflectiveExponent);
};


#endif //IRGLAB_OBJECTMATERIAL_H
