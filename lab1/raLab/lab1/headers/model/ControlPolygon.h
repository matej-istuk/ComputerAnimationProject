//
// Created by babinsinko on 5/20/24.
//

#ifndef IRGLAB_CONTROLPOLYGON_H
#define IRGLAB_CONTROLPOLYGON_H


#include <memory>
#include "Curve.h"
#include "ControlPointChangeListener.h"

class ControlPolygon: public Curve{
private:
    std::vector<glm::vec4> controlPoints;
    std::vector<std::shared_ptr<ControlPointChangeListener>> listeners;
public:
    ControlPolygon();

    void addPoint(glm::vec4);
    void addPoints(std::vector<glm::vec4> points);
    void clearPoints();

    glm::vec4 value(float t) override;

    std::vector<glm::vec4> getLinePoints() override;

    void subscribeListener(const std::shared_ptr<ControlPointChangeListener>& listener);
    void unsubscribeListener(const std::shared_ptr<ControlPointChangeListener>& listener);
    void fireListeners();
};


#endif //IRGLAB_CONTROLPOLYGON_H
