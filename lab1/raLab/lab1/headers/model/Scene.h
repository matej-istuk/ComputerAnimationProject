//
// Created by babinsinko on 27/03/24.
//

#ifndef IRGLAB_SCENE_H
#define IRGLAB_SCENE_H


#include <vector>
#include "Renderable.h"
#include "Object.h"
#include "Renderable.h"
#include "RenderVisitor.h"
#include "Camera.h"
#include "Light.h"

class Scene: public Renderable{
private:
    std::vector<std::shared_ptr<Object>> objects;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Light> light;

public:
    explicit Scene();
    void add(const std::shared_ptr<Object>& object);

    void accept(RenderVisitor& renderVisitor) override;

    const std::vector<std::shared_ptr<Object>> &getObjects() const;

    const std::shared_ptr<Camera> &getCamera() const;

    void setCamera(const std::shared_ptr<Camera> &camera);

    const std::shared_ptr<Light> &getLight() const;

    void setLight(const std::shared_ptr<Light> &light);
};


#endif //IRGLAB_SCENE_H
