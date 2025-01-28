//
// Created by babinsinko on 5/20/24.
//

#ifndef IRGLAB_CONTROLPOINTCHANGELISTENER_H
#define IRGLAB_CONTROLPOINTCHANGELISTENER_H


#include <vector>
#include "glm/vec4.hpp"

class ControlPointChangeListener {
public:
    virtual void controlPointsUpdated(std::vector<glm::vec4> controlPoints) = 0;
};


#endif //IRGLAB_CONTROLPOINTCHANGELISTENER_H
