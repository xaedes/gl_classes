#pragma once

#include <string>
#include <vector>
#include <utility>
#include "gl_classes/imgui_gl.h"
#include "gl_classes/shader.h"

namespace gl_classes {

    struct Program
    {
        Program()
            : Program("", {})
        {}
        Program(const Program& other)
            : m_name(other.getName())
            , m_glProgram(other.getGlProgram())
            , m_shaders(other.getShaders())
            , m_valid(other.isValid())
        {}        
        Program(const std::vector<Shader>& shaders)
            : Program("", shaders)
        {}
        Program(const std::string& name, const std::vector<Shader>& shaders={})
            : m_name(name)
            , m_shaders(shaders)
            , m_valid(false)
        {}
        virtual ~Program(){}

        virtual void setup()
        {
            m_valid = false;
            m_glProgram = glCreateProgram();
            if (m_glProgram != 0)
            {
                m_valid = Link(getGlProgram(), getShaders());
                if (!m_valid)
                {
                    printSourceWithLineNumbers();
                }
            }
            else
            {
                throw std::runtime_error("could not glCreateProgram");
            }
        }

        bool isValid() const { return m_valid; }
        GLuint getGlProgram() const { return m_glProgram; }
        const std::string& getName() const { return m_name; }
        const std::vector<Shader>& getShaders() const { return m_shaders; }
        std::vector<Shader>& getShaders() { return m_shaders; }

        Program withoutShaders() const
        {
            Program result;
            result.m_name = this->m_name;
            result.m_glProgram = this->m_glProgram;
            result.m_valid = this->m_valid;
            return result;
        }
        Program withoutCode() const
        {
            Program result;
            result.m_name = this->m_name;
            result.m_glProgram = this->m_glProgram;
            result.m_valid = this->m_valid;
            result.m_shaders.resize(this->m_shaders.size());
            for(int i = 0; i < result.m_shaders.size(); ++i)
            {
                result.m_shaders.push_back(this->m_shaders[i].withoutCode());
            }
            return result;
        }

        virtual Program& use()
        {
            glUseProgram(getGlProgram());
            return *this;
        }

        void printSourceWithLineNumbers() const
        {
            for (const auto& shader : getShaders())
            {
                std::cout << "Shader (type=" << Shader::TypeString(shader.getType()) << ") '" << shader.getName() << "'" << std::endl;
                shader.printSourceWithLineNumbers();
            }
        }

        static bool Link(GLuint programId, const std::vector<Shader>& shaders)
        {
            for (int i = 0; i < shaders.size(); ++i)
            {
                glAttachShader(programId, shaders[i].getGlShader());
            }
            glLinkProgram(programId);

            GLchar infolog[512];
            glGetProgramInfoLog(programId, 512, nullptr, infolog);
            if (infolog[0] != '\0') 
            {
                std::cout << "Error linking shaders into program! " << std::endl;
                std::cout << infolog << std::endl;
                return false;
            }
            return true;
        }


    protected:
        bool m_valid;
        GLuint m_glProgram;

        std::string m_name;
        std::vector<Shader> m_shaders;
    };


} // namespace gl_classes
