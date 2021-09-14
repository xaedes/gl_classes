#include "gl_classes/imgui_gl.h"
#include "gl_classes/check_gl_error.h"
#include <iostream>

namespace gl_classes {

    void checkGLError()
    {
        GLenum err;
        bool hasError = false;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << err;
            hasError = true;
        }
        if (hasError)
            throw std::runtime_error("Open GL Error");
    }

} // namespace gl_classes
