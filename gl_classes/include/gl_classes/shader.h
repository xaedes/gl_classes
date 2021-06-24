#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <stdexcept>
#include "gl_classes/imgui_gl.h"
#include "gl_classes/replace_string.h"

namespace gl_classes {

    struct Shader
    {
        enum ShaderType : GLenum
        {
            None = 0,
            Vertex = GL_VERTEX_SHADER,
            Geometry = GL_GEOMETRY_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
            Compute = GL_COMPUTE_SHADER
        };

        static std::string TypeString(ShaderType type)
        {
            switch (type)
            {
            case None:     return "None";
            case Vertex:   return "Vertex";
            case Geometry: return "Geometry";
            case Fragment: return "Fragment";
            case Compute:  return "Compute";
            default: throw std::runtime_error("not implemented");
            };
        }

        Shader() : Shader(ShaderType::None, 0, "")
        {}
       
        Shader(const Shader& other)
            : m_valid(other.isValid())
            , m_glShader(other.getGlShader())
            , m_type(other.getType())
            , m_name(other.getName())
            , m_codeTemplate(other.getCodeTemplate())
            , m_code(other.getCode())
        {}

        Shader(ShaderType type, GLuint glShader, const std::string& name="")
            : m_valid(false)
            , m_glShader(glShader)
            , m_type(type)
            , m_name(name)
            , m_codeTemplate("")
            , m_code("")
        {
            m_valid = (glShader != 0);
        }

        Shader(ShaderType type, const std::string& code_template)
            : Shader(type, "", code_template)
        {}

        Shader(ShaderType type, const std::string& name, const std::string& code_template)
            : m_type(type)
            , m_codeTemplate(code_template)
            , m_valid(false)
            , m_glShader(0) // gl shader 0 is invalid shader
        {}

        void setup(const std::vector<std::pair<std::string, std::string>>& replacements = {})
        {
            m_valid = false;
            m_glShader = glCreateShader(static_cast<GLenum>(getType()));
            if (m_glShader != 0)
            {
                replaceStrings(replacements);
                m_valid = Compile(getGlShader(), getCode());
                if (!m_valid)
                {
                    printSourceWithLineNumbers();
                }
            }
            else
            {
                throw std::runtime_error("could not glCreateShader");
            }
        }

        Shader withoutCode() const
        {
            return Shader(getType(), getGlShader(), getName());
        }

        bool isValid() const { return m_valid; }
        GLuint getGlShader() const { return m_glShader; }
        ShaderType getType() const { return m_type; }
        const std::string& getName() const { return m_name; }
        const std::string& getCodeTemplate() const { return m_codeTemplate; }
        const std::string& getCode() const { return m_code; }
        
        void printSourceWithLineNumbers() const
        {
            std::string source(m_code.cbegin(), m_code.cend());
            std::istringstream iss;
            iss.str(source);
            std::string line;
            for (int ln=1; std::getline(iss, line); ++ln) 
            {
                std::cout << ln << "\t" << line << std::endl;
            }
        }

        static bool Compile(GLuint shaderId, const std::string& code)
        {
            char const * codePtr = code.c_str();
            glShaderSource(shaderId, 1, &codePtr , NULL);
            glCompileShader(shaderId);            
            GLchar infolog[512];
            glGetShaderInfoLog(shaderId, 512, nullptr, infolog);
            if (infolog[0] != '\0') 
            {
                std::cout << "Error compiling shader! " << std::endl;
                std::cout << std::string(infolog) << std::endl;
                return false;
            }
            return true;
        }

    protected:
        bool m_valid;
        GLuint m_glShader;
        
        ShaderType m_type;
        std::string m_name;
        std::string m_codeTemplate;
        std::string m_code;

        void replaceStrings(std::vector<std::pair<std::string,std::string>> replacements)
        {
            std::string source(m_codeTemplate.cbegin(), m_codeTemplate.cend());
            std::vector<std::string> removes;
            std::vector<std::string> inserts;
            removes.resize(replacements.size());
            inserts.resize(replacements.size());
            for (int i = 0; i < replacements.size(); ++i)
            {
                removes.push_back(replacements[i].first);
                inserts.push_back(replacements[i].second);
            }
            replace_strings_inplace(source, removes, inserts);
            m_code.clear();
            m_code.insert(m_code.end(), source.cbegin(), source.cend());
        }

    };


} // namespace gl_classes
