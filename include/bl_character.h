#ifndef BL_CHARACTER_H
#define BL_CHARACTER_H

#include <bl_model.h>
#include <vector>
#include <bullet.h>
#include <bl_state.h>
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

class BlCharacter
{
        public:
                BlCharacter(std::vector<BlModel*> *_blModels,
                                float mass,
                                float linearDamping,
                                float angularDamping,
                                float angularThreshold,
                                btConvexShape *_shape,
                                BlState* _blState,
                                btTransform transform);
                ~BlCharacter();
                void handleMovement();
                void handleRotation();
                void loadInBuffer();
                void bindModelMatrix(GLint uniformM);
                void drawCharacter(GLint locModel, GLint locVertices,
                                GLint locNormals, GLint locUVs);

                std::vector<BlModel*> *blModels;
                btRigidBody *rigidBody;
        private:
                btConvexShape* shape;
                BlState* blState;
                float angularThreshold;
};

#endif
