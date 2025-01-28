//
// Created by babinsinko on 5/20/24.
//

#include "model/ApproximationBezierCurve.h"

void ApproximationBezierCurve::controlPointsUpdated(std::vector<glm::vec4> controlPoints) {
    calculateLinePoints(controlPoints);
    setChanged(true);
}
