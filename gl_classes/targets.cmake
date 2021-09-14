cmake_minimum_required(VERSION 3.8)

add_library(
    ${PROJECT_NAME} 
    STATIC 
    src/replace_string.cpp
    src/check_gl_error.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PUBLIC GLEW::glew)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_options(
    ${PROJECT_NAME}
    PUBLIC 
    # glm::vec4().xyz , etc..
    -DGLM_FORCE_SWIZZLE 
    -DGLM_FORCE_INLINE 
)
