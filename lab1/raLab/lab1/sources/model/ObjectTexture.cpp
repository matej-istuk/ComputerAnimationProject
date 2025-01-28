//
// Created by babinsinko on 6/2/24.
//

#include "model/ObjectTexture.h"

unsigned int ObjectTexture::getTextureId() const {
    return textureID;
}

void ObjectTexture::setTextureId(unsigned int textureId) {
    textureID = textureId;
}


void *ObjectTexture::getData() const {
    return data.get();
}

ObjectTexture::ObjectTexture(const std::shared_ptr<unsigned char> &data, int width, int height, int nrChannels) : data(
        data), width(width), height(height), nrChannels(nrChannels) {}

int ObjectTexture::getWidth() const {
    return width;
}

int ObjectTexture::getHeight() const {
    return height;
}

int ObjectTexture::getNrChannels() const {
    return nrChannels;
}
