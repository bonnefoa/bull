#include <bl_input.h>
#include <bl_util.h>
#include <math.h>
#include <bl_log.h>

#define MAX_AXIS 5

BlInput::BlInput()
{
        phi = M_PI;
        theta = M_PI_2;
        fov = M_PI_2;
        speed = 0.001f ;
        mouseSpeed = 0.0001f ;
        position = btVector3(0.f, 4.f, -5.f);
        aspect = 4.0f/3.0f;
        zNear = 0.1f;
        zFar = 100.0f;
        axisRight = 0;
        axisLeft = 0;
        axisUp = 0;
        axisDown = 0;

        sAxisRight = 0;
        sAxisLeft = 0;
        sAxisUp = 0;
        sAxisDown = 0;

        lastTicks = 0 ;
        now = 0;

        projection = computeProjection(fov, aspect, zNear, zFar);
        SDL_SetRelativeMouseMode(SDL_TRUE);
}

void BlInput::handleUp(SDL_Event *event)
{
        switch(event->key.keysym.sym) {
                case SDLK_UP:
                        sAxisUp = 0;
                        axisUp = 0;
                        break;
                case SDLK_DOWN:
                        axisDown = 0;
                        sAxisDown = 0;
                        break;
                case SDLK_LEFT:
                        axisLeft = 0;
                        sAxisLeft = 0;
                        break;
                case SDLK_RIGHT:
                        axisRight = 0;
                        sAxisRight = 0;
                        break;
        }
}

void incrementAxis(SDL_Keymod mod, int *normalAxis, int *modAxis)
{
        if(mod == KMOD_LSHIFT) {
                *modAxis = min(*modAxis+1, MAX_AXIS);
        } else {
                *normalAxis = min(*normalAxis+1, MAX_AXIS);
        }
}

void BlInput::handleDown(SDL_Event *event)
{
        SDL_Keymod mod = SDL_GetModState();
        switch(event->key.keysym.sym) {
                case SDLK_UP:
                        incrementAxis(mod, &axisUp, &sAxisUp);
                        break;
                case SDLK_DOWN:
                        incrementAxis(mod, &axisDown, &sAxisDown);
                        break;
                case SDLK_LEFT:
                        incrementAxis(mod, &axisLeft, &sAxisLeft);
                        break;
                case SDLK_RIGHT:
                        incrementAxis(mod, &axisRight, &sAxisRight);
                        break;
                case SDLK_ESCAPE:
                        state = QUIT;
                        break;
                case SDLK_q:
                        state = QUIT;
                        break;
                case SDLK_r:
                        state = RELOAD;
                        break;
        }
}

void BlInput::handleInput()
{
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT:
                                state = 1;
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
        phi += mouseSpeed * deltaTime * float(deltaX + sAxisRight - sAxisLeft);
        theta += mouseSpeed * deltaTime * float(-deltaY + sAxisUp - sAxisDown);
}

btTransform BlInput::computeView(const btVector3 &right
                , const btVector3 &up
                , const btVector3 &direction
                , const btVector3 &position)
{
        btTransform transform;
        btMatrix3x3 basis;
        basis[0] = right;
        basis[1] = up;
        basis[2] = direction;
        basis = basis.transpose();
        view = btTransform(basis, position);
        view = view.inverse();
        return view;
}

btTransform BlInput::computeProjection(btScalar fov, btScalar aspect,
                btScalar zNear, btScalar zFar)
{
        btScalar f = 1.f / tan(fov / 2.f);
        btScalar xRot = f / aspect;
        btScalar yRot = f;
        btScalar zRot = (zNear + zFar) / (zNear - zFar);
        btScalar zTrans = 2.0f * zNear * zFar / (zNear - zFar);
        btMatrix3x3 basis(xRot, 0, 0
                        , 0, yRot, 0
                        , 0, 0, zRot);
        btVector3 origin = btVector3(0, 0, zTrans);
        return btTransform(basis, origin);
}

void BlInput::logState()
{
        INFO("theta %f, phi %f\n", theta, phi);
        INFO("right %f %f %f\n", right[0], right[1], right[2]);
        INFO("up %f %f %f\n", up[0], up[1], up[2]);
        INFO("direction %f %f %f\n", direction[0], direction[1], direction[2]);
        INFO("position %f %f %f\n", position[0], position[1], position[2]);
        INFO("V\n");
        printBtTransform(&view);
        INFO("P\n");
        printBtTransform(&projection);
}

void BlInput::handleMovement()
{
        float deltaTime;

        deltaTime = getDeltaTime();
        computeNewAngles(deltaTime);

        direction = btVector3(sin(theta) * sin(phi),
                        cos(theta) , sin(theta) * cos(phi));
        right = btVector3(sin(phi - M_PI_2), 0 , cos(phi - M_PI_2));
        up = right.cross(direction);
        computeView(right, up, direction, position);

        position += float(axisDown - axisUp) * direction * deltaTime * speed;
        position += float(axisRight - axisLeft) * right * deltaTime * speed;
}
