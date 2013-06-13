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

void BlNetwork::sendText(unsigned char *text, size_t textSize)
{
        UDPpacket packet;
        packet.data = text;
        packet.len = textSize;
        int numsent = SDLNet_UDP_Send(udpSock, channel, &packet);
        if(!numsent) {
                INFO("udp sent : %s\n", SDLNet_GetError());
        }
}

void BlNetwork::connect()
{
        IPaddress ipAddress;
        SDLNet_ResolveHost(&ipAddress, host, port);
        udpSock = SDLNet_UDP_Open(0);
        if(!udpSock) {
                ERROR("Error opening udp port: %s\n", SDLNet_GetError());
        }
        channel = SDLNet_UDP_Bind(udpSock, -1, &ipAddress);
        if(channel == -1) {
                ERROR("Error on udp bind: %s\n", SDLNet_GetError());
        }
        INFO("Connected to %s on port %i\n", host, port);
}

void BlNetwork::disconnect()
{
        SDLNet_UDP_Unbind(udpSock, channel);
        SDLNet_UDP_Close(udpSock);
        udpSock = NULL;
        channel = -1;
}
