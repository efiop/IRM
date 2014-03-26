#ifndef CAMERA_H
#define CAMERA_H
#include <GL/gl.h>

class Camera
{
public:
    GLfloat XRot;
    GLfloat YRot;
    GLfloat ZRot;

    GLfloat XPos;
    GLfloat YPos;
    GLfloat ZPos;

    GLfloat SpeedFactor;
    GLfloat XSpeed;
    GLfloat YSpeed;
    GLfloat ZSpeed;

    GLfloat field_of_view;
    GLfloat aspect_ratio;
    GLfloat near;
    GLfloat far;

    bool holdingForward;
    bool holdingBackward;
    bool holdingLeftStrafe;
    bool holdingRightStrafe;

    Camera();
    void move(void);
};

#endif // CAMERA_H
