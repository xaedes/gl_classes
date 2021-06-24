#pragma once
#include "gl_classes/imgui_gl.h"
// #include <opencv2/opencv.hpp>

namespace gl_classes {

    /**
     * @brief      This class describes a buffer on device (gpu).
     *
     * @tparam     value_t  Value type, for example glm::vec3
     */
    template <typename value_t>
    class DeviceBuffer
    {
    protected:
        GLenum m_target;
        GLuint m_buffer;

        GLuint m_bufferBase; // only if m_target is one of GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER; see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml
        GLenum m_usage;

        size_t m_numItems;
        size_t m_bufferSize;
        bool m_autoBind = false;

    public:
        using value_type = value_t;
        static constexpr size_t element_size = sizeof(value_type); 

        /**
         * @brief      Constructs a new DeviceBuffer.
         *
         *      target                       |    Purpose
         *      -----------------------------|------------------------------
         *      GL_ARRAY_BUFFER              | Vertex attributes
         *      GL_ATOMIC_COUNTER_BUFFER     | Atomic counter storage
         *      GL_COPY_READ_BUFFER          | Buffer copy source
         *      GL_COPY_WRITE_BUFFER         | Buffer copy destination
         *      GL_DISPATCH_INDIRECT_BUFFER  | Indirect compute dispatch commands
         *      GL_DRAW_INDIRECT_BUFFER      | Indirect command arguments
         *      GL_ELEMENT_ARRAY_BUFFER      | Vertex array indices
         *      GL_PIXEL_PACK_BUFFER         | Pixel read target
         *      GL_PIXEL_UNPACK_BUFFER       | Texture data source
         *      GL_QUERY_BUFFER              | Query result buffer
         *      GL_SHADER_STORAGE_BUFFER     | Read-write storage for shaders
         *      GL_TEXTURE_BUFFER            | Texture data buffer
         *      GL_TRANSFORM_FEEDBACK_BUFFER | Transform feedback buffer
         *      GL_UNIFORM_BUFFER            | Uniform block storage
         * 
         * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
         * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
         *
         * @param[in]  target  Specifies the target to which the buffer object
         *                     is bound, which must be one of the buffer binding
         *                     targets in the above table.
         *                     
         * @param[in]  usage   Specifies the expected usage pattern of the data
         *                     store. The symbolic constant must be
         *                     GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY,
         *                     GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
         *                     GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or
         *                     GL_DYNAMIC_COPY.
         *                     
         * @param[in]  initialCapacity  The initial capacity
         */
        DeviceBuffer(GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW, size_t initialCapacity = 1)
            : m_target(target)
            , m_bufferBase(0)
            , m_buffer(0)
            , m_usage(usage)
            , m_bufferSize(0)
            , m_numItems(initialCapacity)
        {}
        void init() 
        {
            init(m_usage, m_numItems);
        }
        /**
         * @brief      Initialize the buffer.
         *
         * @param[in]  usage            Specifies the expected usage pattern of
         *                              the data store. The symbolic constant
         *                              must be GL_STREAM_DRAW, GL_STREAM_READ,
         *                              GL_STREAM_COPY, GL_STATIC_DRAW,
         *                              GL_STATIC_READ, GL_STATIC_COPY,
         *                              GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or
         *                              GL_DYNAMIC_COPY.
         * @param[in]  numItems         The number of items
         */
        void init(GLenum usage, int numItems)
        {
            m_usage = usage;
            if (m_buffer == 0 ) glGenBuffers(1, &m_buffer);
            resize(numItems);
        }

        void resize(int numItems, bool update_gl=true)
        {
            m_numItems = numItems;
            int newBufSize = element_size * m_numItems;
            // align size to 4 bytes
            if (newBufSize % 4 != 0)
            {
                newBufSize += 4 - (newBufSize % 4);
            }
            
            if (newBufSize > m_bufferSize)
            {
                m_bufferSize = newBufSize;
                if (update_gl)
                {
                    glBindBuffer(m_target, m_buffer);
                    // std::vector<value_type> data(m_numItems);
                    // glBufferData(m_target, m_bufferSize, data.data(), m_usage);
                    glBufferData(m_target, m_bufferSize, NULL, m_usage);
                }
            }
            else
            {
                //m_bufferSize = newBufSize;
            }
        }
        DeviceBuffer<value_type>& bind()
        {
            glBindBuffer(m_target, m_buffer);
            return *this;
        }
        DeviceBuffer<value_type>& upload(const void* data)
        {
            upload(data, 0, m_numItems);
            return *this;
        }
        DeviceBuffer<value_type>& upload(const void* data, int start, int num)
        {
            if (m_autoBind) bind();
            glBufferSubData(m_target, element_size*start, element_size*(num), data);
            return *this;
        }
        DeviceBuffer<value_type>& download(void* data)
        {
            download(data, 0, m_numItems);
            return *this;
        }
        DeviceBuffer<value_type>& download(void* data, int start, int num)
        {
            if (m_autoBind) bind();
            glGetBufferSubData(m_target, element_size*start, element_size*(num), data);
            return *this;
        }
        /**
         * Only if m_target is one of GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER.
         * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml
         */
        GLuint bufferBase() const
        {
            return m_bufferBase;
        }
        /**
         * Only if m_target is one of GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER.
         * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBufferBase.xhtml
         */
        DeviceBuffer<value_type>& bufferBase(GLuint value)
        {
            m_bufferBase = value;
            glBindBufferBase(m_target, m_bufferBase, m_buffer);
            return *this;
        }

        GLenum target() const
        {
            return m_target;
        }

        GLenum usage() const
        {
            return m_usage;
        }

        DeviceBuffer<value_type>& target(GLenum newTarget) 
        {
            m_target = newTarget;
        }

        DeviceBuffer<value_type>& usage(GLenum newUsage) 
        {
            m_usage = newUsage;
        }

        void* map_ro() { return map(GL_READ_ONLY); }
        void* map_wo() { return map(GL_WRITE_ONLY); }
        void* map_rw() { return map(GL_READ_WRITE); }
        void* map(GLenum access)
        {
            glBindBuffer(m_target, m_buffer);
            return glMapBuffer(m_target, access);
        }
        
        void* mapr_ro() { return mapr_ro(0, m_numItems); }
        void* mapr_wo() { return mapr_wo(0, m_numItems); }
        void* mapr_rw() { return mapr_rw(0, m_numItems); }
        void* mapr(GLbitfield access) { return mapr(0, m_numItems, access); }

        void* mapr_ro(int start, int num) { return mapr(start, num, GL_MAP_READ_BIT); }
        void* mapr_wo(int start, int num) { return mapr(start, num, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT); }
        void* mapr_rw(int start, int num) { return mapr(start, num, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT); }
        void* mapr(int start, int num, GLbitfield access)
        {
            glBindBuffer(m_target, m_buffer);
            return glMapBufferRange(m_target, element_size*start, element_size*(num), access);
        }
            // return glMapBufferRange(m_target, access);
        void unmap()
        {
            glUnmapBuffer(m_target);
        }
        int size() const { return m_numItems; }
        GLuint getBufferId() const { return m_buffer; }
        GLuint& getBufferId() { return m_buffer; }

    };

} // namespace gl_classes

