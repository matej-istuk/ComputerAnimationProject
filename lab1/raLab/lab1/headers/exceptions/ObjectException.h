//
// Created by babinsinko on 3/15/24.
//

#ifndef IRGLAB_OBJECTEXCEPTION_H
#define IRGLAB_OBJECTEXCEPTION_H


#include <stdexcept>

class ObjectException: std::exception {
private:
    std::string message;
public:
    explicit ObjectException(const char* msg);
    const char* what() const throw();
};


#endif //IRGLAB_OBJECTEXCEPTION_H
