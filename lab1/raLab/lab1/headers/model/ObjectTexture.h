//
// Created by babinsinko on 6/2/24.
//

#ifndef IRGLAB_OBJECTTEXTURE_H
#define IRGLAB_OBJECTTEXTURE_H


#include <memory>

class ObjectTexture {
private:
    unsigned int textureID{};
    std::shared_ptr<void> data;
    int width;
    int height;
    int nrChannels;
public:
    ObjectTexture(const std::shared_ptr<unsigned char> &data, int width, int height, int nrChannels);

    unsigned int getTextureId() const;
    void *getData() const;
    int getWidth() const;
    int getHeight() const;
    int getNrChannels() const;

    void setTextureId(unsigned int textureId);
};


#endif //IRGLAB_OBJECTTEXTURE_H
