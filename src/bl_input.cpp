#include <bl_input.h>
#include <SDL.h>
#include <math.h>

BlInput::BlInput(): aspect(4.0f/3.0f), zNear(0.1f), zFar(100.0f)
{
        for(int i = 0; i < 322; i++) {
                keys[i] = false;
        }
        phi = 3.14f;
        theta = 0.0f ;
        fov = 45.0f;
        speed = 8.0f ;
        mouseSpeed = 0.1f ;
        lastTicks = 0 ;
        position = btVector3(0.f, 0.f, 0.f);
        computeProjection();
        SDL_SetRelativeMouseMode(SDL_TRUE);
}

void BlInput::pollInput()
{
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                        case SDL_QUIT:
                                gameState = 1;
                                break;
                        case SDL_KEYDOWN:
                                keys[event.key.keysym.sym] = true;
                                break;
                        case SDL_KEYUP:
                                keys[event.key.keysym.sym] = false;
                                break;
                        default:
                                break;
                }
        }
}

btVector3 BlInput::computeCurrentDirection()
{
        btScalar x = sin(theta) * sin(phi);
        btScalar y = cos(theta);
        btScalar z = sin(theta) * cos(phi);
        return btVector3(x, y, z);
}

float BlInput::getDeltaTime()
{
        double currentTicks = SDL_GetTicks();
        float deltaTime = float(currentTicks - lastTicks);
        lastTicks = currentTicks;
        return deltaTime;
}

void BlInput::computeNewAngles(float deltaTime)
{
        int deltaX, deltaY;
        SDL_GetRelativeMouseState(&deltaX, &deltaY);
        phi += mouseSpeed * deltaTime * float(deltaX);
        theta -= mouseSpeed * deltaTime * float(deltaY);
        theta = fmax(0, theta);
        theta = fmin(theta, 3.14);
}

void BlInput::computeView(const btVector3 &lookAt
                , const btVector3 &right
                , const btVector3 &up, const btVector3 &position)
{
        btTransform transform;
        btMatrix3x3 basis;
        basis[0] = right;
        basis[1] = up;
        basis[2] = lookAt;
        view = btTransform(basis.transpose(), position * btScalar(-1));
}

void BlInput::computeProjection()
{
        btMatrix3x3 basis(
                        (cos(fov) / sin(fov)) / aspect, 0, 0
                        , 0, zFar, 0
                        , 0, 0, (zNear + zFar) / (zNear - zFar));
        btVector3 origin = btVector3(0, 0, 2 * zFar * zNear / (zNear - zFar));
        projection = btTransform(basis, origin);
}

void BlInput::handleMovement()
{
        btVector3 direction, up, right;
        float deltaTime;

        deltaTime = getDeltaTime();
        computeNewAngles(deltaTime);

        direction = computeCurrentDirection();
        right = btVector3(sin(phi - 3.14f / 2.0f), 0, cos(phi - 3.14f / 2.0f));
        up = right.cross(direction);

        if(keys[SDLK_UP]) {
                position += direction * deltaTime * speed;
        } else if(keys[SDLK_DOWN]) {
                position -= direction * deltaTime * speed;
        } else if(keys[SDLK_LEFT]) {
                position -= right * deltaTime * speed;
        } else if(keys[SDLK_RIGHT]) {
                position += right * deltaTime * speed;
        }
        computeView(direction, right, up, position);
}

void BlInput::handleInput()
{
        if(keys[SDLK_ESCAPE]) {
                gameState = 1;
        }
}
