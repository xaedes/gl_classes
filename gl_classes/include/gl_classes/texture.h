#pragma once

#include "gl_classes/imgui_gl.h"

namespace gl_classes {

    struct Texture
    {
        /**
         * @brief      Constructs a new instance.
         *
         * @param[in]  textureUnit  Specifies which texture unit to make
         *                          active. The number of texture units is
         *                          implementation dependent, but must be at
         *                          least 80. texture must be one of
         *                          GL_TEXTUREi, where i ranges from zero to the
         *                          value of GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
         *                          minus one. The initial value is GL_TEXTURE0.
         * @param[in]  target       Specifies the target to which the texture is
         *                          bound. Must be one of GL_TEXTURE_1D,
         *                          GL_TEXTURE_2D, GL_TEXTURE_3D,
         *                          GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY,
         *                          GL_TEXTURE_RECTANGLE, GL_TEXTURE_CUBE_MAP,
         *                          GL_TEXTURE_CUBE_MAP_ARRAY,
         *                          GL_TEXTURE_BUFFER, GL_TEXTURE_2D_MULTISAMPLE
         *                          or GL_TEXTURE_2D_MULTISAMPLE_ARRAY.
         */
        Texture(GLenum textureUnit = GL_TEXTURE0, GLenum target = GL_TEXTURE_2D)
        {
            m_textureUnit = textureUnit;
            m_target = target;
        }
        void init()
        {
            init(m_textureUnit, m_target);
        }
        void init(GLenum textureUnit, GLenum target)
        {
            GLuint texture;
            glGenTextures(1, &texture);
            init(textureUnit, target, texture, true);
        }
        void init(GLenum textureUnit, GLenum target, GLuint texture, bool ownsTexture = false)
        {
            if (this->ownsTexture()) deleteTexture();
            m_textureUnit = textureUnit;
            m_target = target;
            m_texture = texture;
            m_ownsTexture = ownsTexture;
            m_initialized = true;
        }
        void deleteTexture()
        {
            if (ownsTexture() && glIsTexture(m_texture))
            {
                // glActiveTexture(m_textureUnit);
                glDeleteTextures(1, &m_texture);
                m_texture = 0;
                m_ownsTexture = false;
                m_initialized = false;
            }
        }
        void bind()
        {
            if (initialized())
            {
                glActiveTexture(m_textureUnit);
                glBindTexture(m_target, m_texture);
            }
        }
        bool initialized() const
        {
            return m_initialized;
        }
        bool ownsTexture() const
        {
            return m_ownsTexture;
        }
        GLenum textureUnit() const
        {
            return m_textureUnit;
        }
        GLenum target() const
        {
            return m_target;
        }
        GLuint textureId() const
        {
            return m_texture;
        }
        void textureUnit(GLenum value)
        {
            m_textureUnit = value;
        }
        void target(GLenum value)
        {
            m_target = value;
        }
        ~Texture()
        {
            if (ownsTexture()) deleteTexture();
        }
    protected:
        GLenum m_textureUnit;
        GLenum m_target;
        GLuint m_texture;
        bool m_initialized = false;
        bool m_ownsTexture = false;
    };


} // namespace gl_classes
