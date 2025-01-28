//
// Created by babinsinko on 27/03/24.
//

#ifndef IRGLAB_OBJECT_H
#define IRGLAB_OBJECT_H


#include <vector>
#include <memory>
#include "Transformable.h"
#include "Shader.h"
#include "Scene.h"
#include "Renderable.h"
#include "RenderVisitor.h"
#include "ObjectMaterial.h"
#include "ObjectTexture.h"

class Object: public Transformable, public Renderable{
private:
    Shader shader;
    std::vector<std::shared_ptr<Renderable>> renderables;
    std::shared_ptr<ObjectMaterial> material;
    std::shared_ptr<ObjectTexture> texture;
public:
    explicit Object(const Shader &shader);

    Object(const Shader &shader, const std::shared_ptr<ObjectMaterial> &material);

    Object(const Shader &shader, const std::shared_ptr<ObjectMaterial> &material,
           const std::shared_ptr<ObjectTexture> &texture);

    void accept(RenderVisitor& renderVisitor) override;
    void add(const std::shared_ptr<Renderable>& renderable);

    const Shader &getShader() const;

    const std::vector<std::shared_ptr<Renderable>> &getRenderables() const;

    const std::shared_ptr<ObjectMaterial> &getMaterial() const;

    void setMaterial(const std::shared_ptr<ObjectMaterial> &material);

    const std::shared_ptr<ObjectTexture> &getTexture() const;

    void setTexture(const std::shared_ptr<ObjectTexture> &texture);

    bool hasMaterial();
    bool hasTexture();

};


#endif //IRGLAB_OBJECT_H
