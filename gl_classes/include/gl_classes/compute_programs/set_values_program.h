#pragma once

#include "glm/glm.hpp"
#include <string>

#include "gl_classes/program.h"
#include "gl_classes/program_uniform.h"
#include "gl_classes/compute_program.h"
#include "gl_classes/shader.h"

namespace gl_classes {
namespace compute_programs {

    template<typename value_t>
    class SetValuesProgram : public gl_classes::ComputeProgram
    {
    public:
        using Program = gl_classes::Program;
        template<class T> using ProgramUniform = gl_classes::ProgramUniform<T>;
        using ComputeProgram = gl_classes::ComputeProgram;
        using Shader = gl_classes::Shader;

        using value_type = value_t;
        SetValuesProgram(){}
        ~SetValuesProgram(){}
        void setup(const std::string& type_str, glm::uvec3 group_size = glm::uvec3(1024,1,1))
        {
            m_group_size = group_size;
            m_shaders = {Shader(Shader::ShaderType::Compute, code())};
            m_shaders[0].setup({
                {"##TYPE##", type_str},
                {"##GROUPSIZE_X##", std::to_string(m_group_size.x)},
                {"##GROUPSIZE_Y##", std::to_string(m_group_size.y)},
                {"##GROUPSIZE_Z##", std::to_string(m_group_size.z)},
            });
            Program::setup();
            num_items.init(getGlProgram(), "num_items");
            value.init(getGlProgram(), "value");
            checkGLError();
        }            
        void dispatch(int num_items, value_type value)
        {
            this->num_items.set(num_items);
            this->value.set(value);
            ComputeProgram::dispatch(num_items, 1, 1, m_group_size.x, m_group_size.y, m_group_size.z);
        }            
        std::string code() const
        {
            return (
        R"(
        #version 440
        #define GROUPSIZE_X ##GROUPSIZE_X##
        #define GROUPSIZE_Y ##GROUPSIZE_Y##
        #define GROUPSIZE_Z ##GROUPSIZE_Z##
        #define GROUPSIZE (GROUPSIZE_X*GROUPSIZE_Y*GROUPSIZE_Z)
        layout(local_size_x=GROUPSIZE_X, local_size_y=GROUPSIZE_Y, local_size_z=GROUPSIZE_Z) in;

        layout (std430, binding = 0) buffer buf
        {
            ##TYPE## values[]; 
        };

        uniform uint num_items;
        uniform ##TYPE## value;

        void main() {
            uint workgroup_idx = 
                gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y +
                gl_WorkGroupID.y * gl_NumWorkGroups.x +
                gl_WorkGroupID.x;
            uint global_idx = gl_LocalInvocationIndex + workgroup_idx * GROUPSIZE;
            if (global_idx >= num_items) return;
            values[global_idx] = value;
        }
        )"
            );
        }            
        ProgramUniform<uint32_t> num_items;
        ProgramUniform<value_type> value;
    protected:
        glm::uvec3 m_group_size;
    };

} // namespace compute_programs
} // namespace gl_classes