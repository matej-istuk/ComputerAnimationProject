//
// Created by babinsinko on 4/1/24.
//

#ifndef IRGLAB_FRUSTUM_H
#define IRGLAB_FRUSTUM_H


class Frustum {
private:
    float fov;
    float aspect;
    float near;
    float far;
    float left;
    float right;
    float bottom;
    float top;

    void update();
public:
    Frustum(float fov, float aspect, float near, float far);
    Frustum();

    float getFov() const;

    void setFov(float fov);

    float getAspect() const;

    void setAspect(float aspect);

    float getNear() const;

    void setNear(float near);

    float getFar() const;

    void setFar(float far);

    float getLeft() const;

    float getRight() const;

    float getBottom() const;

    float getTop() const;
};


#endif //IRGLAB_FRUSTUM_H
