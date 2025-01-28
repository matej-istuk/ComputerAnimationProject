//
// Created by babinsinko on 3/15/24.
//
#ifndef IRGLAB_IOEXCEPTION_H
#define IRGLAB_IOEXCEPTION_H


#include <stdexcept>

class IOException: std::exception {
private:
    std::string message;
public:
    explicit IOException(const char* msg);
    const char* what() const throw();
};

#endif //IRGLAB_IOEXCEPTION_H

