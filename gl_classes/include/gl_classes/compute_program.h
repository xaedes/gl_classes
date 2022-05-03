#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <cstdint>

#include "gl_classes/program.h"

namespace gl_classes {

    class ComputeProgram : public Program
    {
    public:
        ComputeProgram() : Program(){}
        virtual ~ComputeProgram(){}

        virtual void dispatch(uint64_t x, uint64_t y, uint64_t z, uint32_t gx, uint32_t gy, uint32_t gz)
        {
            ComputeProgram::dispatch(
                static_cast<uint32_t>(x/gx + ((x%gx == 0) ? 0 : 1)),
                static_cast<uint32_t>(y/gy + ((y%gy == 0) ? 0 : 1)),
                static_cast<uint32_t>(z/gz + ((z%gz == 0) ? 0 : 1))
            );
        }
        
        virtual void dispatch(uint32_t x, uint32_t y, uint32_t z)
        {
            glDispatchCompute(x, y, z);
            checkGLError();
        }
    };

} // namespace gl_classes
