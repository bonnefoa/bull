#ifndef BL_RENDER_H
#define BL_RENDER_H

#include <GL/glew.h>
#include <GL/gl.h>

class BlRender {
        public:
                BlRender(void);

                void init(void);
                void render(void);

        private:
                GLuint vao;
};

#endif
