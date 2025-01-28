//
// Created by babinsinko on 3/30/24.
//

#ifndef IRGLAB_INITIALIZERRENDERVISITOR_H
#define IRGLAB_INITIALIZERRENDERVISITOR_H


#include "RenderVisitor.h"

class InitializerRenderVisitor: public RenderVisitor {
public:
    void visitScene(Scene& scene) override;
    void visitObject(Object& object) override;
    void visitMesh(Mesh& mesh) override;
    void visitCoordinateSystem(CoordinateSystem& coordinateSystem) override;
    void visitCurve(Curve &curve) override;
};


#endif //IRGLAB_INITIALIZERRENDERVISITOR_H
