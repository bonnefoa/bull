#include <bl_input.h>
#include <bl_util.h>
#include <math.h>
#include <bl_log.h>

#define MAX_AXIS 5
#define TICK_INTERVAL 3000

BlInput::BlInput()
{
        phi = 3.14f;
        theta = 0.0f ;
        fov = 45.0f;
        speed = 0.001f ;
        mouseSpeed = 0.0001f ;
        position = btVector3(0.f, 0.f, -8.f);
        aspect = 4.0f/3.0f;
        zNear = 0.1f;
        zFar = 100.0f;
        axisRight = 0;
        axisLeft = 0;
        axisUp = 0;
        axisDown = 0;

        lastTicks = 0 ;
        now = 0;
        nextTime = 0;

        projection = computeProjection(fov, aspect, zNear, zFar);
        SDL_SetRelativeMouseMode(SDL_TRUE);
}

void BlInput::handleUp(SDL_Event *event)
{
        switch(event->key.keysym.sym) {
                case SDLK_UP:
                        axisUp = 0;
                        break;
                case SDLK_DOWN:
                        axisDown = 0;
                        break;
                case SDLK_LEFT:
                        axisLeft = 0;
                        break;
                case SDLK_RIGHT:
                        axisRight = 0;
                        break;
        }
}

void BlInput::handleDown(SDL_Event *event)
{
        switch(event->key.keysym.sym) {
                case SDLK_UP:
                        axisUp = min(axisUp+1, MAX_AXIS);
                        break;
                case SDLK_DOWN:
                        axisDown = min(axisDown+1, MAX_AXIS);
                        break;
                case SDLK_LEFT:
                        axisLeft = min(axisLeft+1, MAX_AXIS);
                        break;
                case SDLK_RIGHT:
                        axisRight = min(axisRight+1, MAX_AXIS);
                        break;
                case SDLK_ESCAPE:
                        gameState = 1;
        }
}

void BlInput::handleInput()
{
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT:
                                gameState = 1;
                                break;
                        case SDL_KEYDOWN:
                                handleDown(&event);
                                break;
                        case SDL_KEYUP:
                                handleUp(&event);
                                break;
                        default:
                                break;
                }
        }
        handleMovement();
}

float BlInput::getDeltaTime()
{
        now = SDL_GetTicks();
        float deltaTime = float(now - lastTicks);
        lastTicks = now;
        return deltaTime;
}

void BlInput::computeNewAngles(float deltaTime)
{
        int deltaX, deltaY;
        SDL_GetRelativeMouseState(&deltaX, &deltaY);
        phi += mouseSpeed * deltaTime * float(deltaX);
        theta -= mouseSpeed * deltaTime * float(deltaY);
        theta = fmax(0, theta);
        theta = fmin(theta, M_PI);
}

btTransform BlInput::computeView(const btVector3 &lookAt
                , btVector3 &right
                , const btVector3 &eye)
{
        btTransform transform;
        btMatrix3x3 basis;
        btVector3 zAxis = (lookAt - eye).normalize();
        btVector3 xAxis = right.normalize();
        btVector3 yAxis = zAxis.cross(xAxis);
        basis[0] = xAxis;
        basis[1] = yAxis;
        basis[2] = zAxis;
        basis = basis.transpose();
        return btTransform(basis, eye * btScalar(-1));
}

btTransform BlInput::computeProjection(btScalar fov, btScalar aspect, btScalar zNear, btScalar zFar)
{
        btScalar f = 1 / tan(fov/2);
        btScalar xRot = f / aspect;
        btScalar yRot = f;
        btScalar zRot = (zNear + zFar) / (zNear - zFar);
        btScalar zTrans = 2 * zNear * zFar / (zNear - zFar);
        btMatrix3x3 basis(xRot, 0, 0
                        , 0, yRot, 0
                        , 0, 0, zRot);
        btVector3 origin = btVector3(0, 0, zTrans);
        return btTransform(basis, origin);
}

btVector3 BlInput::computeCurrentDirection()
{
        btScalar x = sin(theta) * sin(phi);
        btScalar y = cos(theta);
        btScalar z = sin(theta) * cos(phi);
        return btVector3(x, y, z).normalize();
}

void BlInput::handleMovement()
{
        btVector3 direction, right;
        float deltaTime;

        deltaTime = getDeltaTime();
        computeNewAngles(deltaTime);
        right = btVector3(sin(phi - M_PI_2), 0, cos(phi - M_PI_2));
        direction = computeCurrentDirection();

        if(nextTime <= now) {
                nextTime = now + TICK_INTERVAL;
                INFO("theta %f, phi %f\n", theta, phi);
                INFO("axisUp %d\n", axisUp);
                INFO("right %f %f %f\n", right[0], right[1], right[2]);
                INFO("direction %f %f %f\n", direction[0], direction[1], direction[2]);
        }

        position += float(axisUp - axisDown) * direction * deltaTime * speed;
        position += float(axisRight - axisRight) * right * deltaTime * speed;
        view = computeView(direction, right, position);
}
