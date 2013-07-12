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
                                float speed,
                                btConvexShape *_shape,
                                BlState* _blState,
                                btTransform transform
                                );
                ~BlCharacter();
                void handleMovement();
                void handleTurn(const btQuaternion &direction);
                void loadInBuffer();
                void bindModelMatrix(GLint uniformM);
                void drawCharacter(GLint locModel, GLint locVertices,
                                GLint locNormals, GLint locUVs,
                                GLint locTangent, GLint locBitangent);

                std::vector<BlModel*> *blModels;
                btRigidBody *rigidBody;

                btVector3 getPosition();
                btVector3 getDirection();
                btVector3 getRightDirection();

        private:
                btConvexShape* shape;
                BlState* blState;
                float angularThreshold;
                float speed;
};

#endif
