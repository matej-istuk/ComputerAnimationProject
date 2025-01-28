//
// Created by babinsinko on 5/20/24.
//

#ifndef IRGLAB_INTERPOLATIONBEZIERCURVE_H
#define IRGLAB_INTERPOLATIONBEZIERCURVE_H


#include "BezierCurve.h"
#include "ControlPointChangeListener.h"

class InterpolationBezierCurve: public BezierCurve, public ControlPointChangeListener{
public:
void controlPointsUpdated(std::vector<glm::vec4> controlPoints) override;
};


#endif //IRGLAB_INTERPOLATIONBEZIERCURVE_H
