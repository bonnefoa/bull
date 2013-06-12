#ifndef BL_UVS_H
#define BL_UVS_H

#include <vector>

class BlUvs {
        public:
                BlUvs(std::vector<float> uvsInput,
                                unsigned int numChannelInput
                     ) :
                        uvs(uvsInput),
                        numChannel(numChannelInput) {};

                std::vector<float> uvs;
                unsigned int numChannel;
};

#endif
