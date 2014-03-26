#include "camera.h"

#include <GL/gl.h>
#include <math.h>

#define toRads(x) ((x*3.141592654f)/180.0f)

Camera::Camera() {
    XPos = 10.f;
    YPos = 10.f;
    ZPos = 10.f;

    XRot = 0.f;
    YRot = 0.f;
    ZRot = 0.f;

    SpeedFactor = 3.0f;
    XSpeed = 0.f;
    YSpeed = 0.f;
    ZSpeed = 0.f;

    field_of_view = 45.0;
    aspect_ratio = 1.0;
    near = 0.1;
    far = 1000.0;

    holdingForward     = false;
    holdingBackward    = false;
    holdingLeftStrafe  = false;
    holdingRightStrafe = false;
}

void Camera::move()
{
    float camMovementXComponent = 0.0f;
    float camMovementYComponent = 0.0f;
    float camMovementZComponent = 0.0f;

    if (holdingForward == true)
    {
        float pitchFactor = cos(toRads(XRot));
        camMovementXComponent += (SpeedFactor * float(sin(toRads(YRot))) ) * pitchFactor;

        camMovementYComponent += SpeedFactor * float(sin(toRads(XRot))) * -1.0f;

        float yawFactor = float(cos(toRads(XRot)));
        camMovementZComponent += ( SpeedFactor * float(cos(toRads(YRot))) * -1.0f ) * yawFactor;
    }

    if (holdingBackward == true)
    {
        float pitchFactor = cos(toRads(XRot));
        camMovementXComponent += ( SpeedFactor * float(sin(toRads(YRot))) * -1.0f) * pitchFactor;

        camMovementYComponent += SpeedFactor * float(sin(toRads(XRot)));

        float yawFactor = float(cos(toRads(XRot)));
        camMovementZComponent += (SpeedFactor * float(cos(toRads(YRot))) ) * yawFactor;
    }

    if (holdingLeftStrafe == true)
    {
        float yRotRad = toRads(YRot);

        camMovementXComponent += -SpeedFactor * float(cos(yRotRad));
        camMovementZComponent += -SpeedFactor * float(sin(yRotRad));
    }

    if (holdingRightStrafe == true)
    {
        float yRotRad = toRads(YRot);

        camMovementXComponent += SpeedFactor * float(cos(yRotRad));
        camMovementZComponent += SpeedFactor * float(sin(yRotRad));
    }

    XSpeed = camMovementXComponent;
    YSpeed = camMovementYComponent;
    ZSpeed = camMovementZComponent;

    if (XSpeed > SpeedFactor)
        XSpeed = SpeedFactor;

    if (XSpeed < -SpeedFactor)
        XSpeed = -SpeedFactor;

    if (YSpeed > SpeedFactor)
        YSpeed = SpeedFactor;

    if (YSpeed < -SpeedFactor)
        YSpeed = -SpeedFactor;

    if (ZSpeed > SpeedFactor)
        ZSpeed = SpeedFactor;

    if (ZSpeed < -SpeedFactor)
        ZSpeed = -SpeedFactor;

    XPos += XSpeed;
    YPos += YSpeed;
    ZPos += ZSpeed;


    glRotatef(XRot, 1.0f, 0.0f, 0.0f);
    glRotatef(YRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(-XPos,-YPos,-ZPos);
}
