#pragma once
#include "gl_classes/imgui_gl.h"
#include <string>
#include <glm/glm.hpp>
// #include <opencv2/opencv.hpp>

namespace gl_classes {

    template <typename T>
    class ProgramUniform
    {
    public:
        ProgramUniform() {}
        void init(GLuint glProgram, const std::string& name, const T& value)
        {
            init(glProgram, name);
            set(value);
        }
        void init(GLuint glProgram, const std::string& name)
        {
            m_glProgram = glProgram;
            m_name = name;
            m_nameZeroed = m_name + '\0';
            m_loc = glGetUniformLocation(m_glProgram, m_nameZeroed.c_str());
            if (m_enableDebugOutput)
            {
                std::cout << "location for " << m_name << " is " << m_loc << std::endl;
                if (m_loc < 0)
                {
                    std::cout << "location for uniform " << m_name << " not found!" << std::endl;
                }
            }
        }
        void set(const T& value);
        inline const T& get() const { return m_value; }

    public:
        T m_value;
        GLuint m_glProgram;
        GLint m_loc = -1;
        std::string m_name;
        std::string m_nameZeroed;
        bool m_enableDebugOutput = false;
    };

    // full template full specialization is no longer a template. It's a
    // concrete function. As such it needs to be (implicitly or explicitly)
    // declared inline. See https://stackoverflow.com/a/4447057/798588

    template<> inline void ProgramUniform<bool>::set(const bool& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glProgramUniform.xhtml
            // Either the i, ui or f variants may be used to provide values for
            // uniform variables of type bool, bvec2, bvec3, bvec4, or arrays of
            // these. The uniform variable will be set to false if the input
            // value is 0 or 0.0f, and it will be set to true otherwise.
            glProgramUniform1ui(m_glProgram, m_loc, value ? 1 : 0);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<float>::set(const float& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform1f(m_glProgram, m_loc, m_value);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<unsigned>::set(const unsigned& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform1ui(m_glProgram, m_loc, m_value);
        }
        else if (m_enableDebugOutput)    {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<int>::set(const int& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform1i(m_glProgram, m_loc, m_value);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::vec2>::set(const glm::vec2& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform2fv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::vec3>::set(const glm::vec3& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform3fv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::vec4>::set(const glm::vec4& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform4fv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::ivec2>::set(const glm::ivec2& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform2iv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::ivec3>::set(const glm::ivec3& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform3iv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::ivec4>::set(const glm::ivec4& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform4iv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::uvec2>::set(const glm::uvec2& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform2uiv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::uvec3>::set(const glm::uvec3& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform3uiv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::uvec4>::set(const glm::uvec4& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            glProgramUniform4uiv(m_glProgram, m_loc, 1, &m_value[0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::mat2>::set(const glm::mat2& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            bool transpose = false;
            glProgramUniformMatrix2fv(m_glProgram, m_loc, 1, transpose, &m_value[0][0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::mat3>::set(const glm::mat3& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            bool transpose = false;
            glProgramUniformMatrix3fv(m_glProgram, m_loc, 1, transpose, &m_value[0][0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    template<> inline void ProgramUniform<glm::mat4>::set(const glm::mat4& value)
    {
        m_value = value;
        if (m_loc != -1) 
        {
            bool transpose = false;
            glProgramUniformMatrix4fv(m_glProgram, m_loc, 1, transpose, &m_value[0][0]);
        }
        else if (m_enableDebugOutput)
        {
            std::cout << "location for uniform " << m_name << " not found!" << std::endl;
        }
    }
    // template<> inline void ProgramUniform<cv::Matx44f>::set(const cv::Matx44f& value)
    // {
    //     m_value = value;
    //     if (m_loc != -1) 
    //     {
    //         bool transpose = false;
    //         glProgramUniformMatrix4fv(m_glProgram, m_loc, 1, transpose, &m_value(0,0));
    //     }
    //     else if (m_enableDebugOutput)
    //     {
    //         std::cout << "location for uniform " << m_name << " not found!" << std::endl;
    //     }
    // }

} // namespace gl_classes
