#ifndef BL_NETWORK_H
#define BL_NETWORK_H

#include <SDL_net.h>
#include <bl_config.h>

class BlNetwork {
        public:
                BlNetwork(BlConfig *_blConfig) :
                        blConfig(_blConfig) {};
                virtual ~BlNetwork ();

                void init();
                void connect();
                void disconnect();
        private:
                BlConfig *blConfig;
                UDPsocket udpSock;
};

#endif
