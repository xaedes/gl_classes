#pragma once

#include "imgui.h"
#include "imgui_stdlib.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h> // Access to more advanced ImGui variables.

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <cstdint>

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

inline void checkGLError()
{
    GLenum err;
    bool hasError = false;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << err;
        hasError = true;
    }
    if (hasError)
        throw std::runtime_error("Open GL Error");
}

template<class U>
struct ImGuiDataTypeOfPtr_
{
    template <class T> ImGuiDataType operator()( T*   val );
    template <> ImGuiDataType operator()( int8_t*   val ) { return ImGuiDataType_S8;     }
    template <> ImGuiDataType operator()( uint8_t*  val ) { return ImGuiDataType_U8;     }
    template <> ImGuiDataType operator()( int16_t*  val ) { return ImGuiDataType_S16;    }
    template <> ImGuiDataType operator()( uint16_t* val ) { return ImGuiDataType_U16;    }
    template <> ImGuiDataType operator()( int32_t*  val ) { return ImGuiDataType_S32;    }
    template <> ImGuiDataType operator()( uint32_t* val ) { return ImGuiDataType_U32;    }
    template <> ImGuiDataType operator()( int64_t*  val ) { return ImGuiDataType_S64;    }
    template <> ImGuiDataType operator()( uint64_t* val ) { return ImGuiDataType_U64;    }
    template <> ImGuiDataType operator()( float*    val ) { return ImGuiDataType_Float;  }
    template <> ImGuiDataType operator()( double*   val ) { return ImGuiDataType_Double; }
};

template<typename T>
bool ImGuiSliderScalar(const char* label, T* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGui::SliderScalar(label, ImGuiDataTypeOfPtr_<T>(p_data), p_data, p_min, p_max, format, flags);
}
template<typename T>
bool ImGuiSliderScalar(const char* label, T* p_data, T p_min, T p_max, const char* format = NULL, ImGuiSliderFlags flags = 0)
{
    return ImGuiSliderScalar(label, p_data, &p_min, &p_max, format, flags);
}

inline bool ImGuiButton(const char* label, bool enabled = true, const ImVec2& size = ImVec2(0, 0))
{
    // https://github.com/ocornut/imgui/issues/1889#issuecomment-398681105
    if (!enabled)
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    bool result = ImGui::Button(label, size);
    if (!enabled)
    {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    return result;
}

template <class...Args>
void ImGuiText(const std::string& str, Args&&... args)
{
    ImGui::Text(str.c_str(), std::forward<Args>(args)...);
}
