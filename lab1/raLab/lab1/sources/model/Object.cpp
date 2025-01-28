//
// Created by babinsinko on 27/03/24.
//

#include "model/Object.h"

void Object::accept(RenderVisitor& renderVisitor) {
    renderVisitor.visitObject(*this);
    for (const auto &item: renderables) {
        item->accept(renderVisitor);
    }
}

Object::Object(const Shader &shader) : Object(shader, std::shared_ptr<ObjectMaterial>()) {}



const Shader &Object::getShader() const {
    return shader;
}

const std::vector<std::shared_ptr<Renderable>> &Object::getRenderables() const {
    return renderables;
}

void Object::add(const std::shared_ptr<Renderable>& renderable) {
    renderables.push_back(renderable);
}

const std::shared_ptr<ObjectMaterial> &Object::getMaterial() const {
    return material;
}

void Object::setMaterial(const std::shared_ptr<ObjectMaterial> &material) {
    Object::material = material;
}

Object::Object(const Shader &shader, const std::shared_ptr<ObjectMaterial> &material) :
    Transformable(),
    shader(shader),
    renderables({}),
    material(material) {}

bool Object::hasMaterial() {
    return material != nullptr;
}

Object::Object(const Shader &shader, const std::shared_ptr<ObjectMaterial> &material,
               const std::shared_ptr<ObjectTexture> &texture) : shader(shader), material(material), texture(texture) {}

const std::shared_ptr<ObjectTexture> &Object::getTexture() const {
    return texture;
}

void Object::setTexture(const std::shared_ptr<ObjectTexture> &texture) {
    Object::texture = texture;
}

bool Object::hasTexture() {
    return texture != nullptr;
}
