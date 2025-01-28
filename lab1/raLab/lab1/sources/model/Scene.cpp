//
// Created by babinsinko on 27/03/24.
//

#include "model/Scene.h"

Scene::Scene():
        objects() {}


void Scene::accept(RenderVisitor& renderVisitor) {
    renderVisitor.visitScene(*this);
    for (const auto &item: objects) {
        item->accept(renderVisitor);
    }
}

const std::vector<std::shared_ptr<Object>> &Scene::getObjects() const {
    return objects;
}

void Scene::add(const std::shared_ptr<Object>& object) {
    objects.push_back(object);
}

const std::shared_ptr<Camera> &Scene::getCamera() const{
    return camera;
}

void Scene::setCamera(const std::shared_ptr<Camera> &camera) {
    Scene::camera = camera;
}

const std::shared_ptr<Light> &Scene::getLight() const {
    return light;
}

void Scene::setLight(const std::shared_ptr<Light> &light) {
    Scene::light = light;
}
