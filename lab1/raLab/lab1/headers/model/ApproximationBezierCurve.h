//
// Created by babinsinko on 5/20/24.
//

#ifndef IRGLAB_APPROXIMATIONBEZIERCURVE_H
#define IRGLAB_APPROXIMATIONBEZIERCURVE_H


#include "BezierCurve.h"
#include "ControlPointChangeListener.h"

class ApproximationBezierCurve: public BezierCurve, public ControlPointChangeListener{
public:
    void controlPointsUpdated(std::vector<glm::vec4> controlPoints) override;
};


#endif //IRGLAB_APPROXIMATIONBEZIERCURVE_H
