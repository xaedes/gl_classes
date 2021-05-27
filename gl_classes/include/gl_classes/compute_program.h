#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>

#include "gl_classes/program.h"

namespace gl_classes {

    class ComputeProgram : public Program
    {
    public:
        ComputeProgram() : Program(){}
        virtual ~ComputeProgram(){}
        virtual ComputeProgram& dispatch(int x, int y, int z, int gx, int gy, int gz)
        {
            return ComputeProgram::dispatch(
                x/gx + ((x%gx == 0) ? 0 : 1),
                y/gy + ((y%gy == 0) ? 0 : 1),
                z/gz + ((z%gz == 0) ? 0 : 1)
            );
        }
        
        virtual ComputeProgram& dispatch(int x, int y, int z)
        {
            glDispatchCompute(x, y, z);
            checkGLError();
            return *this;
        }
    };

} // namespace gl_classes
