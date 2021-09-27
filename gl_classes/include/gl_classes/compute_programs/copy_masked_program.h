#pragma once

#include "glm/glm.hpp"
#include <string>

#include "gl_classes/program.h"
#include "gl_classes/program_uniform.h"
#include "gl_classes/compute_program.h"
#include "gl_classes/shader.h"

namespace gl_classes {
namespace compute_programs {

    class CopyMaskedProgram : public gl_classes::ComputeProgram
    {
    public:
        using Program = gl_classes::Program;
        template<class T> using ProgramUniform = gl_classes::ProgramUniform<T>;
        using ComputeProgram = gl_classes::ComputeProgram;
        using Shader = gl_classes::Shader;

        inline CopyMaskedProgram(){}
        inline ~CopyMaskedProgram(){}
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
            offset_mask.init(getGlProgram(), "offset_mask", 0);
            offset_out.init(getGlProgram(), "offset_out", 0);
            checkGLError();
        }            
        inline void dispatch(uint32_t num_items, uint32_t offset_in = 0, uint32_t offset_mask = 0, uint32_t offset_out = 0)
        {
            this->num_items.set(num_items);
            this->offset_in.set(offset_in);
            this->offset_mask.set(offset_mask);
            this->offset_out.set(offset_out);
            ComputeProgram::dispatch(num_items, 1, 1, m_group_size.x, m_group_size.y, m_group_size.z);
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
        layout (std430, binding = 1) buffer buf_mask
        {
            uint mask[]; 
        };
        layout (std430, binding = 2) buffer buf_out_data
        {
            ##DATA_TYPE## out_data[]; 
        };
        layout (std430, binding = 3) buffer buf_out_count
        {
            uint out_count[]; 
        };

        uniform uint num_items;
        uniform uint offset_in;
        uniform uint offset_mask;
        uniform uint offset_out;

        void main() {
            uint workgroup_idx = 
                gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y +
                gl_WorkGroupID.y * gl_NumWorkGroups.x +
                gl_WorkGroupID.x;
            uint global_idx = gl_LocalInvocationIndex + workgroup_idx * GROUPSIZE;
            if (global_idx >= num_items) return;
            
            if (mask[offset_mask + global_idx] != 0)
            {
                uint idx = atomicAdd(out_count[0], 1);
                out_data[offset_out + idx] = data[offset_in + global_idx];
            }
        }
        )"
            );
        }            
        ProgramUniform<uint32_t> num_items;
        ProgramUniform<uint32_t> offset_in;
        ProgramUniform<uint32_t> offset_mask;
        ProgramUniform<uint32_t> offset_out;
    protected:
        glm::uvec3 m_group_size;
    };

} // namespace compute_programs
} // namespace gl_classes