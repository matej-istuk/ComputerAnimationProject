//
// Created by babinsinko on 3/15/24.
//

#include "exceptions/ObjectException.h"

ObjectException::ObjectException(const char *msg)
        : message(msg)
{}

const char *ObjectException::what() const throw() {
    return message.c_str();
}
