#ifndef BL_CHARACTER_H
#define BL_CHARACTER_H

#include <bl_model.h>
#include <vector>

class BlCharacter
{
        public:
                BlCharacter(std::vector<BlModel*> _blModels)
                        : blModels(_blModels) { };
                ~BlCharacter();

        private:
                std::vector<BlModel*> blModels;
};

#endif
