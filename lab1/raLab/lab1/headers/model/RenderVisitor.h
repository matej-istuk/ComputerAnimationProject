//
// Created by babinsinko on 3/30/24.
//

#ifndef IRGLAB_RENDERVISITOR_H
#define IRGLAB_RENDERVISITOR_H



class Scene;
class Object;
class Mesh;
class CoordinateSystem;
class Curve;

class RenderVisitor {
public:
    virtual void visitScene(Scene& scene) = 0;
    virtual void visitObject(Object& object) = 0;
    virtual void visitMesh(Mesh& mesh) = 0;
    virtual void visitCoordinateSystem(CoordinateSystem& coordinateSystem) = 0;
    virtual void visitCurve(Curve& curve) = 0;
};


#endif //IRGLAB_RENDERVISITOR_H
