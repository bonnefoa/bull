#ifndef BL_NETWORK_H
#define BL_NETWORK_H

#include <SDL_net.h>
#include <bl_config.h>

class BlNetwork {
        public:
                BlNetwork(BlConfig *blConfig) :
                        host(blConfig->host),
                        port(blConfig->port) {};
                BlNetwork(const char *_host,
                                int _port) :
                        host(_host),
                        port(_port) {};
                virtual ~BlNetwork ();

                void init();
                void connect();
                void disconnect();
                void sendText(unsigned char *txt, size_t textLenght);
        private:
                UDPsocket udpSock;
                const char *host;
                int port;
                int channel;
};

#endif
