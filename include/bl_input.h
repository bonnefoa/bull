#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

class BlInput
{

        public:
                bool keys[322];
                int gameState;

                BlInput();
                void pollInput();
                void handleInput();
};


#endif
