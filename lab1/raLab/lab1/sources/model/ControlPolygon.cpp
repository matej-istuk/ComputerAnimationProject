//
// Created by babinsinko on 5/20/24.
//

#include <algorithm>
#include "model/ControlPolygon.h"

ControlPolygon::ControlPolygon(): Curve(), controlPoints({}), listeners({}){}

std::vector<glm::vec4> ControlPolygon::getLinePoints() {
    return controlPoints;
}

void ControlPolygon::addPoint(glm::vec4 point) {
    setChanged(true);
    controlPoints.push_back(point);
    fireListeners();
}

void ControlPolygon::subscribeListener(const std::shared_ptr<ControlPointChangeListener>& listener) {
    listeners.push_back(listener);
}

void ControlPolygon::unsubscribeListener(const std::shared_ptr<ControlPointChangeListener>& listener) {
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void ControlPolygon::fireListeners() {
    for (std::shared_ptr<ControlPointChangeListener> &listener: listeners) {
        listener->controlPointsUpdated(controlPoints);
    }
}

glm::vec4 ControlPolygon::value(float t) {
    return {};
}

void ControlPolygon::addPoints(std::vector<glm::vec4> points) {
    for (glm::vec4 item: points) {
        this->addPoint(item);
    }

}

void ControlPolygon::clearPoints() {
    controlPoints.clear();
}
