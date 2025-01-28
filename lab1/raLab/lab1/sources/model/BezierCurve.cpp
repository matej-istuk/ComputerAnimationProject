//
// Created by babinsinko on 5/20/24.
//

#include "model/BezierCurve.h"
#include <cmath>

std::vector<glm::vec4> BezierCurve::getLinePoints() {
    return linePoints;
}

int BezierCurve::getPointsPerSegment() const {
    return samplesPerSegment;
}

void BezierCurve::setPointsPerSegment(int samplesPerSegment) {
    BezierCurve::samplesPerSegment = samplesPerSegment;
}

void BezierCurve::calculateLinePoints(std::vector<glm::vec4> controlPoints) {
    BezierCurve::controlPoints = controlPoints;
    linePoints.clear();
    if (controlPoints.size() <= 1) {
        return;
    }
    int numSamples = (controlPoints.size() - 1) * samplesPerSegment + controlPoints.size();
    for (int sample = 0; sample < numSamples; sample++) {
        linePoints.push_back(value((float) sample / (numSamples - 1)));
    }
}

int nChoosek( int n, int k )
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

double BezierCurve::weightFunction(int n, int i, float t) {
    return nChoosek(n, i) * pow(t, i) * pow(1 - t, n - i);
}

BezierCurve::BezierCurve(int samplesPerSegment) : samplesPerSegment(samplesPerSegment) {}

BezierCurve::BezierCurve(): BezierCurve(10) {}

glm::vec4 BezierCurve::value(float t) {
    glm::vec4 samplePoint = glm::vec4(0);
    for (int i = 0; i < controlPoints.size(); i++) {
        samplePoint += controlPoints[i] * glm::vec4(weightFunction(controlPoints.size() - 1, i, t));
    }
    return samplePoint;
}

