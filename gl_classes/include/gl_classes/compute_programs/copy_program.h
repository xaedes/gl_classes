#pragma once

#include "glm/glm.hpp"
#include <string>

#include "gl_classes/program.h"
#include "gl_classes/program_uniform.h"
#include "gl_classes/compute_program.h"
#include "gl_classes/shader.h"

namespace gl_classes {
namespace compute_programs {

    class CopyProgram : public gl_classes::ComputeProgram
    {
    public:
        using Program = gl_classes::Program;
        template<class T> using ProgramUniform = gl_classes::ProgramUniform<T>;
        using ComputeProgram = gl_classes::ComputeProgram;
        using Shader = gl_classes::Shader;

        inline CopyProgram(){}
        inline ~CopyProgram(){}
        inline void setup(
            const std::string& data_type_str, 
            glm::uvec3 group_size = glm::uvec3(1024,1,1)
        )
        {
            m_group_size = group_size;
            m_shaders = {Shader(Shader::ShaderType::Compute, code())};
            m_shaders[0].setup({
                {"##DATA_TYPE##", data_type_str},
                {"##GROUPSIZE_X##", std::to_string(m_group_size.x)},
                {"##GROUPSIZE_Y##", std::to_string(m_group_size.y)},
                {"##GROUPSIZE_Z##", std::to_string(m_group_size.z)},
            });
            Program::setup();
            num_items.init(getGlProgram(), "num_items");
            offset_in.init(getGlProgram(), "offset_in", 0);
            offset_out.init(getGlProgram(), "offset_out", 0);
            checkGLError();
        }            
        void dispatch(uint32_t num_items)
        {
            this->num_items.set(num_items);
            ComputeProgram::dispatch(num_items, 1, 1, m_group_size.x, m_group_size.y, m_group_size.z);
        }
        void dispatch(uint32_t num_items, uint32_t offset_in, uint32_t offset_out)
        {
            this->offset_in.set(offset_in);
            this->offset_out.set(offset_out);
            dispatch(num_items);
        }
        inline std::string code() const
        {
            return (
        R"(
        #version 440
        #define GROUPSIZE_X ##GROUPSIZE_X##
        #define GROUPSIZE_Y ##GROUPSIZE_Y##
        #define GROUPSIZE_Z ##GROUPSIZE_Z##
        #define GROUPSIZE (GROUPSIZE_X*GROUPSIZE_Y*GROUPSIZE_Z)
        layout(local_size_x=GROUPSIZE_X, local_size_y=GROUPSIZE_Y, local_size_z=GROUPSIZE_Z) in;

        layout (std430, binding = 0) buffer buf_data
        {
            ##DATA_TYPE## data[]; 
        };
        layout (std430, binding = 1) buffer buf_out_data
        {
            ##DATA_TYPE## out_data[]; 
        };

        uniform uint num_items;
        uniform uint offset_in;
        uniform uint offset_out;

        void main() {
            uint workgroup_idx = 
                gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y +
                gl_WorkGroupID.y * gl_NumWorkGroups.x +
                gl_WorkGroupID.x;
            uint global_idx = gl_LocalInvocationIndex + workgroup_idx * GROUPSIZE;
            if (global_idx >= num_items) return;
            
            out_data[offset_out + global_idx] = data[offset_in + global_idx];
        }
        )"
            );
        }            
        ProgramUniform<uint32_t> num_items;
        ProgramUniform<uint32_t> offset_in;
        ProgramUniform<uint32_t> offset_out;
    protected:
        //void dispatch(uint32_t x, uint32_t y, uint32_t z) override
        //{
        //    // prevent public access to CopyProgram::dispatch(uint32_t, uint32_t, uint32_t)
        //    // as this is once was accessible as a public member in this class which set two additional uniform values.
        //    // to avoid confusion/conflict with ComputeProgram::dispatch(uint32_t,uint32_t,uint32_t) it was changed to accept
        //    // num_items only.
        //    // if you used this function, set offset_in and offset_out explicitly instead.
        //    ComputeProgram::dispatch(x, y, z);
        //    return *static_cast<ComputeProgram*>(this);
        //}

        glm::uvec3 m_group_size;
    };

} // namespace compute_programs
} // namespace gl_classes