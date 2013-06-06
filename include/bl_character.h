#ifndef BL_CHARACTER_H
#define BL_CHARACTER_H

#include <bl_model.h>
#include <vector>
#include <bullet.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

class BlCharacter
{
        public:
                BlCharacter(std::vector<BlModel*> *_blModels,
                                btConvexShape *_shape);
                ~BlCharacter();
                void loadInBuffer();

        private:
                std::vector<BlModel*> *blModels;
                btConvexShape* shape;
                btPairCachingGhostObject* ghostObject;
                btKinematicCharacterController *controller;
};

#endif
