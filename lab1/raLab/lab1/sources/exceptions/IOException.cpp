//
// Created by babinsinko on 3/15/24.
//

#include "exceptions/IOException.h"

IOException::IOException(const char *msg)
        : message(msg)
{}

const char *IOException::what() const throw() {
    return message.c_str();
}
