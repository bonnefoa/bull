#ifndef BL_TEXT_UTIL_H
#define BL_TEXT_UTIL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <bl_program_text2d.h>
#include <bl_program_text3d.h>
#include <bl_state.h>
#include <bl_config.h>
#include <vector>
#include <bullet.h>

class BlText
{
        public:
                BlText(
                                BlProgramText2d *_blProgramText2d,
                                BlProgramText3d *_blProgramText3d,
                                BlConfig *_blConfig,
                                BlState *_blState)
                        :
                                blProgramText2d(_blProgramText2d),
                                blProgramText3d(_blProgramText3d),
                                blConfig(_blConfig),
                                blState(_blState) {};
                ~BlText();

                void init();
                void print2dText(const char *text, const float x, const float y);
                void print3dText(const btVector3& location,const char* textString);
                void fillTextTexture(const char *text, float *width, float *height);

                void bindUV(GLint locUV);

                GLuint textureBuffer;
        private:
                BlProgramText2d *blProgramText2d;
                BlProgramText3d *blProgramText3d;
                BlConfig *blConfig;
                BlState *blState;

                GLuint vertexBuffer;
                GLuint uvBuffer;

                std::vector<float> textUvs;
};

#endif
