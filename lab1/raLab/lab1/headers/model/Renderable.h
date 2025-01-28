//
// Created by babinsinko on 3/30/24.
//

#ifndef IRGLAB_RENDERABLE_H
#define IRGLAB_RENDERABLE_H


#include "RenderVisitor.h"

class Renderable {
public:
    virtual void accept(RenderVisitor& renderVisitor) = 0;
    virtual ~Renderable() = default;
};


#endif //IRGLAB_RENDERABLE_H
