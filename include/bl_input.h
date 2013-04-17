#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include "btBulletDynamicsCommon.h"

class BlInput
{

        public:
                bool keys[322];
                int gameState;

                BlInput();
                void pollInput();
                void handleInput();

                btMatrix3x3 view;
                btMatrix3x3 projection;
};


#endif
