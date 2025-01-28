//
// Created by babinsinko on 5/20/24.
//

#include "model/InterpolationBezierCurve.h"
#include "glm/mat4x4.hpp"

void InterpolationBezierCurve::controlPointsUpdated(std::vector<glm::vec4> controlPoints) {
    if (controlPoints.size() == 3) {
        glm::mat3 inv = glm::mat3(0);
        for (int t = 0; t < 3; t++) {
            for (int i = 0; i < 3; i++) {
                inv[t][i] = weightFunction(2, i, t / 2.);
            }
        }

        glm::mat3x4 points = glm::mat3x4(0);
        points[0] = controlPoints[0];
        points[1] = controlPoints[1];
        points[2] = controlPoints[2];

        glm::mat3x4 newPoints = (points) * glm::inverse(inv);
        calculateLinePoints({newPoints[0], newPoints[1], newPoints[2]});
    } else if (controlPoints.size() >= 4) {
        glm::mat4 inv = glm::mat4(0);
        for (int t = 0; t < 4; t++) {
            for (int i = 0; i < 4; i++) {
                inv[t][i] = weightFunction(3, i, t / 3.);
            }
        }

        glm::mat4 points = glm::mat4(0);
        points[0] = controlPoints[controlPoints.size() - 4];
        points[1] = controlPoints[controlPoints.size() - 3];
        points[2] = controlPoints[controlPoints.size() - 2];
        points[3] = controlPoints[controlPoints.size() - 1];

        glm::mat4 newPoints = (points) * glm::inverse(inv);
        calculateLinePoints({newPoints[0], newPoints[1], newPoints[2], newPoints[3]});
    } else {
        calculateLinePoints(controlPoints);
    }
    setChanged(true);
}
