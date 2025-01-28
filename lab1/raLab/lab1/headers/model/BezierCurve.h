//
// Created by babinsinko on 5/20/24.
//

#ifndef IRGLAB_BEZIERCURVE_H
#define IRGLAB_BEZIERCURVE_H


#include "Curve.h"

class BezierCurve: public Curve{
private:
    std::vector<glm::vec4> linePoints;
    std::vector<glm::vec4> controlPoints;
    int samplesPerSegment;

public:
    BezierCurve();

    explicit BezierCurve(int samplesPerSegment);

    static double weightFunction(int n, int i, float t);

    std::vector<glm::vec4> getLinePoints() override;

    glm::vec4 value(float t) override;

    int getPointsPerSegment() const;
    void setPointsPerSegment(int samplesPerSegment);

    void calculateLinePoints(std::vector<glm::vec4> controlPoints);
};


#endif //IRGLAB_BEZIERCURVE_H
