//
// Created by babinsinko on 5/20/24.
//


#include "model/BCurve.h"
#include "glm/detail/type_mat4x4.hpp"

const glm::mat4 BCurve::BCURVE_MATRIX = glm::mat4(
        glm::vec4(-1./6, 3./6, -3./6, 1./6),
        glm::vec4(3./6, -6./6, 0, 4./6),
        glm::vec4(-3./6, 3./6, 3./6, 1./6),
        glm::vec4(1./6, 0, 0, 0)
        );

const glm::mat4x3 BCurve::BCURVE_DERIVATIVE_MATRIX = glm::mat4x3(
        glm::vec3(-1./2, 2./2, -1./2),
        glm::vec3(3./2, -4./2, 0.),
        glm::vec3(-3./2, 2./2, 1./2),
        glm::vec3(1./2, 0., 0.)
);

const glm::mat4x2 BCurve::BCURVE_SECOND_DERIVATIVE_MATRIX = glm::mat4x2(
        glm::vec2(-1., 1.),
        glm::vec2(3.,-2.),
        glm::vec2(-3.,1.),
        glm::vec2(1.,0.)
);

BCurve::BCurve(): BCurve(40) {}

BCurve::BCurve(int samplesPerSegment): samplesPerSegment(samplesPerSegment) {}

std::vector<glm::vec4> BCurve::getLinePoints() {
    return BCurve::linePoints;
}

glm::vec4 BCurve::value(float t) {
    t = std::max(std::min(t, (float) this->getNumSegments()), 0.f);
    int segment = (int) t;
    t = t - segment;

    return glm::vec4(t * t * t, t * t, t, 1)
    * BCURVE_MATRIX
    * glm::transpose(glm::mat4(controlPoints[segment], controlPoints[segment + 1], controlPoints[segment + 2], controlPoints[segment + 3]));
}

int BCurve::getPointsPerSegment() const {
    return samplesPerSegment;
}

void BCurve::setPointsPerSegment(int samplesPerSegment) {
    BCurve::samplesPerSegment = samplesPerSegment;
}

void BCurve::calculateLinePoints(std::vector<glm::vec4> controlPoints) {
    BCurve::controlPoints = controlPoints;
    linePoints.clear();
    if (controlPoints.size() < 4) {
        return;
    }
    int numSegments = this->getNumSegments();
    int numSamples = numSegments * samplesPerSegment + 1;
    for (int sample = 0; sample < numSamples; sample++) {
        linePoints.push_back(value((float) sample / samplesPerSegment));
    }
}

int BCurve::getNumSegments() const {
    return BCurve::controlPoints.size() - 3;
}

void BCurve::controlPointsUpdated(std::vector<glm::vec4> controlPoints) {
    calculateLinePoints(controlPoints);
    setChanged(true);
}

glm::vec4 BCurve::derivativeValue(float t) {
    t = std::max(std::min(t, (float) this->getNumSegments()), 0.f);
    int segment = (int) t;
    t = t - segment;

    return glm::vec3(t * t, t, 1)
           * BCURVE_DERIVATIVE_MATRIX
           * glm::transpose(glm::mat4(controlPoints[segment], controlPoints[segment + 1], controlPoints[segment + 2], controlPoints[segment + 3]));
}

glm::vec4 BCurve::secondDerivativeValue(float t) {
    t = std::max(std::min(t, (float) this->getNumSegments()), 0.f);
    int segment = (int) t;
    t = t - segment;

    return glm::vec2(t, 1)
           * BCURVE_SECOND_DERIVATIVE_MATRIX
           * glm::transpose(glm::mat4(controlPoints[segment], controlPoints[segment + 1], controlPoints[segment + 2], controlPoints[segment + 3]));
}

