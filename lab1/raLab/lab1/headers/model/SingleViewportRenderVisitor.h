//
// Created by babinsinko on 3/30/24.
//

#ifndef IRGLAB_SINGLEVIEWPORTRENDERVISITOR_H
#define IRGLAB_SINGLEVIEWPORTRENDERVISITOR_H


#include "RenderVisitor.h"
#include <memory>
#include "Camera.h"
#include "Light.h"

class SingleViewportRenderVisitor: public RenderVisitor {
private:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Light> light;
    Object* activeObject;
    int polygonMode;
public:
    SingleViewportRenderVisitor();

    void visitScene(Scene& scene) override;
    void visitObject(Object& object) override;
    void visitMesh(Mesh& mesh) override;
    void visitCoordinateSystem(CoordinateSystem& coordinateSystem) override;
    void visitCurve(Curve &curve) override;
};


#endif //IRGLAB_SINGLEVIEWPORTRENDERVISITOR_H
