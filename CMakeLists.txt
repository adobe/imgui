cmake_minimum_required(VERSION 3.16)

# A simple cmake setup for the main library and the glfw/opengl3 backend.
# Waiting on https://github.com/ocornut/imgui/pull/3027 for proper support. 

project(imgui)

option(IMGUI_USE_DARK_THEME "Use Dark ImGui Spectrum Theme" OFF)
option(IMGUI_DEFINE_MATH_OPERATORS "Enable ImGui ImVec2/ImVec4 operators" OFF)
option(IMGUI_BUILD_EXAMPLES "" OFF)

add_library(imgui 
    "${imgui_SOURCE_DIR}/imgui.h"
    "${imgui_SOURCE_DIR}/imgui_internal.h"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_spectrum.h"
    "${imgui_SOURCE_DIR}/imgui_spectrum.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h"
    "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
)
add_library(imgui::imgui ALIAS imgui)

if(IMGUI_USE_DARK_THEME)
    target_compile_definitions(imgui PUBLIC SPECTRUM_USE_DARK_THEME)
endif()
if(IMGUI_DEFINE_MATH_OPERATORS)
    target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
endif()
target_compile_definitions(imgui PUBLIC
    IMGUI_IMPL_OPENGL_LOADER_GL3W=1
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS # to check for obsolete functions
    # IMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/path/to/imconfig.h" # to use your own imconfig.h
)
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(imgui PUBLIC cxx_std_11)

# the following should go in different files, but we're keeping things here to avoid future conflicts. 
if(NOT TARGET glfw)
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG tags/3.3
        GIT_SHALLOW TRUE
    )

    option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
    option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
    option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
    option(GLFW_INSTALL "Generate installation target" OFF)
    option(GLFW_VULKAN_STATIC "Use the Vulkan loader statically linked into application" OFF)
    FetchContent_MakeAvailable(glfw)
endif()

target_link_libraries(imgui PUBLIC glfw)

if(IMGUI_BUILD_EXAMPLES)
    add_executable(imgui_example
        "examples/example_glfw_opengl3/main.cpp")
    target_link_libraries(imgui_example PRIVATE imgui)
    target_include_directories(imgui_example PRIVATE "backends/")
    find_package(OpenGL REQUIRED)
    if(TARGET OpenGL::OpenGL)
        target_link_libraries(imgui_example PRIVATE OpenGL::OpenGL)
    else()
        target_link_libraries(imgui_example PRIVATE OpenGL::GL)
    endif()
endif()
