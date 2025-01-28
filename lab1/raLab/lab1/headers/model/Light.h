//
// Created by babinsinko on 5/21/24.
//

#ifndef IRGLAB_LIGHT_H
#define IRGLAB_LIGHT_H


#include "model/Transformable.h"
#include "glm/detail/type_vec3.hpp"

class Light: public Transformable{
private:
    glm::vec3 ambientIntensity{};
    glm::vec3 diffuseIntensity{};
    glm::vec3 specularIntensity{};
public:
    Light(const glm::vec3 &ambientIntensity, const glm::vec3 &diffuseIntensity, const glm::vec3 &specularIntensity);
    Light();
    explicit Light(float strength);

    const glm::vec3 &getAmbientIntensity() const;

    void setAmbientIntensity(const glm::vec3 &ambientIntensity);

    const glm::vec3 &getDiffuseIntensity() const;

    void setDiffuseIntensity(const glm::vec3 &diffuseIntensity);

    const glm::vec3 &getSpecularIntensity() const;

    void setSpecularIntensity(const glm::vec3 &specularIntensity);
};


#endif //IRGLAB_LIGHT_H
