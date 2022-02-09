#pragma once
#include "gl_classes/imgui_gl.h"
#include <vector>
#include <glm/glm.hpp>
// #include <opencv2/opencv.hpp>

#include "gl_classes/device_buffer.h"

namespace gl_classes {

    /**
     * @brief      This class describes a buffer on device (gpu).
     *
     * @tparam     value_t  Value type, for example glm::vec3
     */
    template <typename value_t, typename buffer_t = std::vector<value_t>>
    class HostDeviceBuffer : public DeviceBuffer<value_t>
    {
    protected:
    public:
        using value_type = value_t;
        using buffer_type = buffer_t;

        //HostDeviceBuffer(const HostDeviceBuffer& other) = default;
        //    //: DeviceBuffer(other)
        //    //, buffer(other.buffer)
        ////{}
        //HostDeviceBuffer& operator=(const HostDeviceBuffer& other)
        //{
        //    DeviceBuffer::operator=(other);
        //    buffer = other->buffer;
        //    return *this;
        //}

        HostDeviceBuffer(GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW, size_t initialSize = 0,  size_t initialCapacity = 1)
            : DeviceBuffer(target, usage, initialCapacity)
            , buffer(initialSize)
        {
            buffer.reserve(initialCapacity);
        }
        buffer_type buffer;

        HostDeviceBuffer& upload()
        {
            if (buffer.size() != DeviceBuffer::size())
            {
                DeviceBuffer::resize(buffer.size());
            }
            DeviceBuffer::upload(buffer.data(), 0, buffer.size());
            return *this;
        }
        HostDeviceBuffer& download()
        {
            buffer.resize(DeviceBuffer::size());
            DeviceBuffer::download(buffer.data());
            return *this;
        }
        HostDeviceBuffer& upload(size_t start, size_t num)
        {
            if (start + num > DeviceBuffer::size())
            {
                upload();
            }
            else
            {
                DeviceBuffer::upload(buffer.data() + start, start, num);
            }
            return *this;
        }
        HostDeviceBuffer& download(size_t start, size_t num)
        {
            if (start+num > buffer.size())
            {
                download();
            }
            else
            {
                DeviceBuffer::download(buffer.data() + start, start, num);
            }
            return *this;
        }
        HostDeviceBuffer& upload(const void* data, size_t start, size_t num)
        {
            DeviceBuffer::upload(data, start, num);
            return *this;
        }
        HostDeviceBuffer& download(void* data, size_t start, size_t num)
        {
            DeviceBuffer::download(data, start, num);
            return *this;
        }
    };

} // namespace gl_classes

