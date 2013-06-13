#include "bl_network.h"
#include <bl_log.h>

BlNetwork::~BlNetwork()
{
        SDLNet_Quit();
}

void BlNetwork::init()
{
        if(SDLNet_Init()==-1) {
                ERROR("Error on sdl net initialization: %s\n", SDLNet_GetError());
        }
}

void BlNetwork::connect()
{
        IPaddress ipAddress;
        SDLNet_ResolveHost(&ipAddress, blConfig->host, blConfig->port);
        udpSock = SDLNet_UDP_Open(0);
        if(!udpSock) {
                ERROR("Error opening udp port: %s\n", SDLNet_GetError());
        }

        int channel = SDLNet_UDP_Bind(udpSock, -1, &ipAddress);
        if(channel == -1) {
                ERROR("Error on udp bind: %s\n", SDLNet_GetError());
        }
}

void BlNetwork::disconnect()
{
        SDLNet_UDP_Close(udpSock);
        udpSock = NULL;
}
