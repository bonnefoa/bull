#ifndef BL_STATE_H
#define BL_STATE_H

enum gamestate_enum {
        NORMAL,
        QUIT,
        RELOAD,
        STOP,
};

class BlState {
        public:
                BlState(gamestate_enum _gamestate) :
                        gamestate(_gamestate) {} ;
                gamestate_enum gamestate;
};

#endif
