#ifndef BL_TEXT_UTIL_H
#define BL_TEXT_UTIL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program_text2d.h>
#include <bl_state.h>

class BlText
{
        public:
                BlText(BlProgramText2d *_blProgramText2d,
                                BlState *_blState)
                        : blProgramText2d(_blProgramText2d),
                          blState(_blState)
                          {};
                ~BlText();

                void init();
                void print2dText(const char *text, const int x, const int y);

        private:
                BlProgramText2d *blProgramText2d;
                BlState *blState;

                GLuint textureBuffer;

                GLuint vertexBuffer;
                GLuint uvBuffer;
};

#endif
