#pragma once

#include <string>
#include <vector>
#include <utility>
#include <stdexcept>
#include <stdint.h> 

#include "gl_classes/imgui_gl.h"

namespace gl_classes {

    class VertexArray
    {
    public:

        struct VertexAttribPointer
        {
            VertexAttribPointer(
                GLuint bufferId = 0,
                GLint size = 1,
                GLenum type = GL_FLOAT,
                GLsizei typeSize = sizeof(float),
                GLboolean normalized = GL_FALSE,
                GLsizei stride = 0,
                void * offset = 0,
                GLuint divisor = 0
            )
                : bufferId(bufferId)
                , size(size)
                , type(type)
                , typeSize(typeSize)
                , normalized(normalized)
                , stride(stride)
                , offset(offset)
                , divisor(divisor)
            {}

            void init(GLuint arrayId, GLuint attribId)
            {
                this->attribId = attribId;
                init(arrayId);
            }

            void init(GLuint arrayId)
            {
                enable(arrayId);
                // glBindBuffer: If pointer is not NULL, a non-zero named buffer
                // object must be bound to the GL_ARRAY_BUFFER target (see
                // glBindBuffer), otherwise an error is generated.
                glBindBuffer(GL_ARRAY_BUFFER, bufferId);
                glVertexAttribPointer(
                    attribId,
                    size,
                    type,
                    normalized,
                    stride,
                    offset
                );
                glVertexAttribDivisor(attribId, divisor);
            }

            void enable(GLuint arrayId)
            {
                glEnableVertexArrayAttrib(arrayId, attribId);
            }
            void disable(GLuint arrayId)
            {
                glDisableVertexArrayAttrib(arrayId, attribId);
            }

            GLuint bufferId;
            GLuint attribId;
            GLint size;
            GLenum type;
            GLsizei typeSize;
            GLboolean normalized;
            GLsizei stride;
            void * offset;
            GLuint divisor;
        };

        VertexArray()
        {}


        void init(const std::vector<VertexAttribPointer>& attribs, bool genVertexArray = true)
        {
            setAttribs(attribs);
            init(genVertexArray);

        }
        void init(bool genVertexArray = true)
        {
            if (genVertexArray && (m_vertexArrayId == 0))
            {
                glGenVertexArrays(1, &m_vertexArrayId);
            }
            bind();
            for (int i=0; i<m_attribs.size(); ++i)
            {
                m_attribs[i].init(m_vertexArrayId, i);
            }
        }
        
        void bind()
        {
            glBindVertexArray(m_vertexArrayId);
        }

        void unbind()
        {
            glBindVertexArray(0);
        }

        GLuint vertexArrayId() { return m_vertexArrayId; };

        const std::vector<VertexAttribPointer>& attribs() const { return m_attribs; }
        void setAttribs(const std::vector<VertexAttribPointer>& attribs)
        { 
            m_attribs.clear();
            for (int i=0; i<attribs.size(); ++i)
            {
                const auto& attr = attribs[i]; 
                
                // glVertexAttribPointer only allows 4 floats size, so we need to pass each column seperate if we need more 
                // also see https://gamedev.stackexchange.com/a/149561/14704
                auto attrib_size = attr.size;
                int k=0;
                bool split = attrib_size > 4;
                while (attrib_size > 0)
                {
                    VertexAttribPointer attr_part;
                    attr_part.size = (attrib_size < 4) ? attrib_size : 4;
                    attr_part.bufferId = attr.bufferId;
                    attr_part.type = attr.type;
                    attr_part.typeSize = attr.typeSize;
                    attr_part.normalized = attr.normalized;
                    // use stride of actual (unsplitted attr), if it is zero, we must
                    // explicitly put in the implicit stride of the original attr.
                    // otherwise the implicit stride would be calculated from at most 4
                    // elements of this attr_patr
                    attr_part.stride = (split /* || (attr.stride == 0) */ ) ? (attr.size * attr.typeSize) : attr.stride;
                    // You can't perform pointer arithmetic on a void * because void doesn't have a defined size.
                    attr_part.offset = ((int8_t*)attr.offset + k * attr_part.size * attr_part.typeSize);
                    //attr_part.offset = attr.offset + k * attr_part.size * attr_part.typeSize;
                    attr_part.divisor = attr.divisor;
                    m_attribs.push_back(attr_part);
                    ++k;
                    attrib_size -= attr_part.size;
                }
            }
        }

    protected:
        std::vector<VertexAttribPointer> m_attribs;
        GLuint m_vertexArrayId = 0;
    };

} // namespace gl_classes
