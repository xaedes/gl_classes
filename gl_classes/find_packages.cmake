cmake_minimum_required(VERSION 3.8)

#                             # imported target
#-----------------------------#----------------
find_package(OpenGL REQUIRED) # OpenGL::GL
find_package(glm REQUIRED)    # glm::glm
find_package(glfw3 REQUIRED)  # glfw
find_package(glew REQUIRED)   # GLEW::glew
if (NOT TARGET imgui AND NOT TARGET imgui::imgui)
    find_package(imgui REQUIRED)  # imgui::imgui
endif()
