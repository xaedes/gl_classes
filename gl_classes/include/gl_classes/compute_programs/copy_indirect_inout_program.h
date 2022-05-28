#pragma once

#include "glm/glm.hpp"
#include <string>

#include "gl_classes/program.h"
#include "gl_classes/program_uniform.h"
#include "gl_classes/compute_program.h"
#include "gl_classes/shader.h"

namespace gl_classes {
namespace compute_programs {

    class CopyIndirectInoutProgram : public gl_classes::ComputeProgram
    {
    public:
        using Program = gl_classes::Program;
        template<class T> using ProgramUniform = gl_classes::ProgramUniform<T>;
        using ComputeProgram = gl_classes::ComputeProgram;
        using Shader = gl_classes::Shader;

        inline CopyIndirectInoutProgram(){}
        inline ~CopyIndirectInoutProgram(){}
        inline void setup(
            const std::string& indirection_type_str, 
            const std::string& data_type_str, 
            glm::uvec3 group_size = glm::uvec3(1024,1,1)
        )
        {
            m_group_size = group_size;
            m_shaders = {Shader(Shader::ShaderType::Compute, code())};
            m_shaders[0].setup({
                {"##INDIRECTION_TYPE##", indirection_type_str},
                {"##DATA_TYPE##", data_type_str},
                {"##GROUPSIZE_X##", std::to_string(m_group_size.x)},
                {"##GROUPSIZE_Y##", std::to_string(m_group_size.y)},
                {"##GROUPSIZE_Z##", std::to_string(m_group_size.z)},
            });
            Program::setup();
            num_items.init(getGlProgram(), "num_items");
            num_data.init(getGlProgram(), "num_data");
            offset_in_data.init(getGlProgram(), "offset_in_data", 0);
            offset_out_data.init(getGlProgram(), "offset_out_data", 0);
            symmetric.init(getGlProgram(), "symmetric", false);
            use_input_indirection.init(getGlProgram(), "use_input_indirection", true);
            use_output_indirection.init(getGlProgram(), "use_output_indirection", true);
            checkGLError();
        }            
        inline void dispatch(uint32_t num_items, uint32_t num_data)
        {
            this->num_items.set(num_items);
            this->num_data.set(num_data);
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

        layout (std430, binding = 0) buffer buf0 { ##INDIRECTION_TYPE## in_indirection[];  };
        layout (std430, binding = 1) buffer buf1 { ##INDIRECTION_TYPE## out_indirection[];  };
        layout (std430, binding = 2) buffer buf2 { ##DATA_TYPE## in_data[];  };
        layout (std430, binding = 3) buffer buf3 { ##DATA_TYPE## out_data[];  };

        uniform uint num_items;
        uniform uint num_data;
        uniform uint offset_in_data = 0;
        uniform uint offset_out_data = 0;
        uniform bool symmetric = false;
        uniform bool use_input_indirection = true;
        uniform bool use_output_indirection = true;

        void main() {
            uint workgroup_idx = 
                gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y +
                gl_WorkGroupID.y * gl_NumWorkGroups.x +
                gl_WorkGroupID.x;
            uint global_idx = gl_LocalInvocationIndex + workgroup_idx * GROUPSIZE;
            if (global_idx >= num_items) return;
            
            ##INDIRECTION_TYPE## in_ind  = !use_input_indirection  ? global_idx : in_indirection[global_idx];
            ##INDIRECTION_TYPE## out_ind = !use_output_indirection ? global_idx : (symmetric ? in_ind : out_indirection[global_idx]);
            
            if (
                (in_ind < 0)
             || (in_ind >= num_data)
             || (out_ind < 0)
             || (out_ind >= num_data)
            ) return;

            out_data[offset_out_data + out_ind] = in_data[offset_in_data + in_ind];
        }
        )"
            );
        }            
        ProgramUniform<uint32_t> num_items;
        ProgramUniform<uint32_t> num_data;
        ProgramUniform<uint32_t> offset_in_data;
        ProgramUniform<uint32_t> offset_out_data;
        ProgramUniform<bool> symmetric;
        ProgramUniform<bool> use_input_indirection;
        ProgramUniform<bool> use_output_indirection;
    protected:
        glm::uvec3 m_group_size;
    };

} // namespace compute_programs
} // namespace gl_classes