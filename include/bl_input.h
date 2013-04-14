#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

class BlInput
{

        public:
                bool keys[322];
                int game_state;

                BlInput();
                void poll_input();
                void handle_input();
};


#endif
