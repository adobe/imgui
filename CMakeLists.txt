cmake_minimum_required(VERSION 3.16)

# A simple cmake setup for the main library and some backends.

project(imgui)

option(IMGUI_USE_DARK_THEME "Use Dark ImGui Spectrum Theme" OFF)
option(IMGUI_DEFINE_MATH_OPERATORS "Enable ImGui ImVec2/ImVec4 operators" OFF)
option(IMGUI_BUILD_EXAMPLES "" OFF)
option(IMGUI_ENABLE_STDLIB "Enable imgui_stdlib header" ON)
set(IMGUI_BACKEND_RENDERER "opengl3" CACHE STRING "Which renderer backend to use?")
set_property(CACHE IMGUI_BACKEND_RENDERER PROPERTY STRINGS none allegro5 dx9 dx10 dx11 dx12 metal opengl2 opengl3 sdlrenderer2 sdlrenderer3 vulkan wgpu)
set(IMGUI_BACKEND_PLATFORM "glfw" CACHE STRING "Which platform backend to use?")
set_property(CACHE IMGUI_BACKEND_PLATFORM PROPERTY STRINGS none allegro5 android glfw glut osx sdl2 sdl3 win32)

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
)
add_library(imgui::imgui ALIAS imgui)

if(IMGUI_ENABLE_STDLIB)
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h" "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp")
endif()

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

# ###########
# BACKENDS #
# ###########
if(IMGUI_BACKEND_RENDERER STREQUAL "none")
# do nothing. imgui will compile without a renderer.
elseif(IMGUI_BACKEND_RENDERER STREQUAL "allegro5")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_allegro5.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_allegro5.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx9")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.cpp")
    target_link_libraries(imgui PRIVATE d3d9)
elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx10")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_dx10.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_dx10.cpp")
    target_link_libraries(imgui PRIVATE d3d10)
elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx11")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp")
    target_link_libraries(imgui PRIVATE d3d11)
elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx12")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp")
    target_link_libraries(imgui PRIVATE d3d12 dxgi)
elseif(IMGUI_BACKEND_RENDERER STREQUAL "metal")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_metal.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_metal.mm")
    target_link_libraries(imgui PRIVATE "-framework Metal -framework QuartzCore")
elseif(IMGUI_BACKEND_RENDERER STREQUAL "opengl2")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp")

    find_package(OpenGL REQUIRED)
    if(TARGET OpenGL::OpenGL)
        target_link_libraries(imgui PRIVATE OpenGL::OpenGL)
    elseif(TARGET OpenGL::GL)
        target_link_libraries(imgui PRIVATE OpenGL::GL)
    endif()
elseif(IMGUI_BACKEND_RENDERER STREQUAL "opengl3")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")

    find_package(OpenGL REQUIRED)
    if(TARGET OpenGL::OpenGL)
        target_link_libraries(imgui PRIVATE OpenGL::OpenGL)
    elseif(TARGET OpenGL::GL)
        target_link_libraries(imgui PRIVATE OpenGL::GL)
    endif()
elseif(IMGUI_BACKEND_RENDERER STREQUAL "sdlrenderer2")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_RENDERER STREQUAL "sdlrenderer3")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_RENDERER STREQUAL "vulkan")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_RENDERER STREQUAL "wgpu")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp")
    # untested, needs work
endif()

if(IMGUI_BACKEND_PLATFORM STREQUAL "none")
    # do nothing. imgui will compile without a platform.
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "allegro5")
    # already included by the renderer backend above
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "android")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/android.h" "${imgui_SOURCE_DIR}/backends/android.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "glfw")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")

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
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "glut")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_glut.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_glut.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "osx")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_osx.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_osx.mm")
    target_link_libraries(imgui PRIVATE "-framework AppKit -framework MetalKit -framework Foundation -framework GameController")
    # This builds successfully, but hangs on startup. Needs work.
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "sdl2")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "sdl3")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp")
    # untested, needs work
elseif(IMGUI_BACKEND_PLATFORM STREQUAL "win32")
    target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp")
endif()

if(IMGUI_BUILD_EXAMPLES)
    if(IMGUI_BACKEND_RENDERER STREQUAL "none" AND IMGUI_BACKEND_PLATFORM STREQUAL "none")
        add_executable(imgui_example "examples/example_null/main.cpp")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "opengl2" AND IMGUI_BACKEND_PLATFORM STREQUAL "glfw")
        add_executable(imgui_example "examples/example_glfw_opengl2/main.cpp")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "opengl3" AND IMGUI_BACKEND_PLATFORM STREQUAL "glfw")
        add_executable(imgui_example "examples/example_glfw_opengl3/main.cpp")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx9" AND IMGUI_BACKEND_PLATFORM STREQUAL "win32")
        add_executable(imgui_example "examples/example_win32_directx9/main.cpp")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx10" AND IMGUI_BACKEND_PLATFORM STREQUAL "win32")
        add_executable(imgui_example "examples/example_win32_directx10/main.cpp")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx11" AND IMGUI_BACKEND_PLATFORM STREQUAL "win32")
        add_executable(imgui_example "examples/example_win32_directx11/main.cpp")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "dx12" AND IMGUI_BACKEND_PLATFORM STREQUAL "win32")
        add_executable(imgui_example "examples/example_win32_directx12/main.cpp")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "opengl3" AND IMGUI_BACKEND_PLATFORM STREQUAL "win32")
        add_executable(imgui_example "examples/example_win32_opengl3/main.cpp")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "metal" AND IMGUI_BACKEND_PLATFORM STREQUAL "glfw")
        add_executable(imgui_example "examples/example_glfw_metal/main.mm")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "metal" AND IMGUI_BACKEND_PLATFORM STREQUAL "osx")
        add_executable(imgui_example "examples/example_apple_metal/main.mm")
    elseif(IMGUI_BACKEND_RENDERER STREQUAL "opengl2" AND IMGUI_BACKEND_PLATFORM STREQUAL "osx")
        add_executable(imgui_example "examples/example_apple_opengl2/main.mm")
    endif()

    if(TARGET imgui_example)
        target_link_libraries(imgui_example PRIVATE imgui)
        target_include_directories(imgui_example PRIVATE "backends/")
    else()
        message(WARNING "IMGUI_BUILD_EXAMPLES is set, but example was not built. \nUnsupported or not-yet-implemented combination of renderer and platform. \nSet IMGUI_BUILD_EXAMPLES=OFF to disable this warning.")
    endif()
endif()
