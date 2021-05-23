#pragma once

#include <vector>
#include <iostream>

namespace gl_classes {

template < class T, class Alloc = std::allocator<T> >  
class NonShrinkingVector : public std::vector<T, Alloc>
{
public:
    using std::vector<T, Alloc>::vector;

    size_type size() const noexcept
    {
        return m_insertIndex;
    }

    void push_back(const value_type& val)
    {
        if (m_insertIndex < vector::size())
        {
            (*this)[m_insertIndex] = val;
        }
        else
        {
            vector::push_back(val);
            //std::cout << ".";
            // std::cout << "push_back [" << this << "]\n";
        }
        ++m_insertIndex;
    }

    void push_back (value_type&& val)
    {
        if (m_insertIndex < vector::size())
        {
            (*this)[m_insertIndex] = val;
        }
        else
        {
            vector::push_back(std::move(val));
            // std::cout << ".";
        }
        ++m_insertIndex;
    }

    void clear() noexcept
    {
        // std::cout << "clear [" << this << "]\n";
        m_insertIndex = 0;
    }

protected:
    size_t m_insertIndex = 0;
};

} // namespace gl_classes
