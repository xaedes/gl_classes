#pragma once

#include <array>

namespace gl_classes {

    template <typename T>
    class PingPong
    {
    public:
        T& write_toggle() { /*return write(); toggle(); --> */ toggle(); return read(); }

        T& read() { return m_buffers[m_readIdx]; }
        T& write() { return m_buffers[1-m_readIdx]; }
        void toggle() { m_readIdx = 1-m_readIdx; }
        T& get(int idx) { return m_buffers[idx]; }
        void setRead(int idx) { m_readIdx = idx; }
        T& operator[] (int idx) { return m_buffers[idx]; }
        
    protected:
        int m_readIdx=0;
        std::array<T, 2> m_buffers;
    };

} // namespace gl_classes
