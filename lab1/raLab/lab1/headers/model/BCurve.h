//
// Created by babinsinko on 5/20/24.
//

#ifndef IRGLAB_BCURVE_H
#define IRGLAB_BCURVE_H


#include "Curve.h"
#include "glm/fwd.hpp"
#include "ControlPointChangeListener.h"

class BCurve: public Curve, public ControlPointChangeListener{
private:
    std::vector<glm::vec4> linePoints;
    std::vector<glm::vec4> controlPoints;
    int samplesPerSegment;

public:
    static const glm::mat4 BCURVE_MATRIX;
    static const glm::mat4x3 BCURVE_DERIVATIVE_MATRIX;
    static const glm::mat4x2 BCURVE_SECOND_DERIVATIVE_MATRIX;

    BCurve();

    explicit BCurve(int samplesPerSegment);

    std::vector<glm::vec4> getLinePoints() override;

    glm::vec4 value(float t) override;

    glm::vec4 derivativeValue(float t);
    glm::vec4 secondDerivativeValue(float t);

    int getPointsPerSegment() const;
    void setPointsPerSegment(int samplesPerSegment);

    int getNumSegments() const;
    void calculateLinePoints(std::vector<glm::vec4> controlPoints);

    void controlPointsUpdated(std::vector<glm::vec4> controlPoints) override;
};


#endif //IRGLAB_BCURVE_H
