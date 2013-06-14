#include "bl_camera.h"
#include <bl_matrix.h>
#include <bl_log.h>
#include <bl_util.h>

BlCamera::~BlCamera()
{
}

void BlCamera::computeDirectionFromDelta(float &deltaUp, float &deltaRight)
{
        btQuaternion rightRot = btQuaternion(btVector3(0,1,0), deltaRight);
        btQuaternion upRot = btQuaternion(rightDirection, deltaUp);

        btQuaternion center = btQuaternion(btVector3(0, 1, 0), rotation.getAxis()[1] * rotation.getAngle());
        float angle = center.angle(center.nearest(upRot * rotation));
        float maxAngle = blConfig->lookThreshold / 4.0f;

        if(angle < maxAngle) {
                rotation = upRot * rotation;
        }

        rotation = rightRot * rotation ;

        btTransform trans = btTransform(rotation);

        rightDirection = trans(btVector3(1, 0, 0));
        direction = trans(btVector3(0,0,-1));
        upDirection = trans(btVector3(0, 1, 0));
}

void BlCamera::moveCamera(btVector3 newPosition)
{
        switch(cameraState) {
                case FIRST_PERSON:
                        position = newPosition + direction;
                        break;
                case THIRD_PERSON:
                        position = newPosition - direction * 4.0f;
                        break;
        }
}

void BlCamera::computeNewCamera()
{
        float deltaUp = blConfig->mouseSpeed * blState->deltaTime *
                blState->getDeltaRotY();
        float deltaRight = -1.0f * blConfig->mouseSpeed * blState->deltaTime *
                blState->getDeltaRotX();
        computeDirectionFromDelta(deltaUp, deltaRight);

        view = computeViewMatrix(rightDirection, upDirection,
                        direction, position);
}
