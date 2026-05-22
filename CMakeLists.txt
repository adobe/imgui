cmake_minimum_required(VERSION 3.16)

# A simple cmake setup for the main library and some backends.

project(imgui)

option(IMGUI_USE_DARK_THEME "Use Dark ImGui Spectrum Theme" OFF)
option(IMGUI_DEFINE_MATH_OPERATORS "Enable ImGui ImVec2/ImVec4 operators" OFF)
option(IMGUI_BUILD_EXAMPLES "" OFF)
option(IMGUI_ENABLE_STDLIB "Enable imgui_stdlib header" ON)
# Renderer / platform backend selection. Accepts a single value or a CMake list
# (semicolon-separated) to enable multiple backends in one build, e.g.
#   -DIMGUI_BACKEND_RENDERER="opengl3;vulkan" -DIMGUI_BACKEND_PLATFORM="glfw;win32"
set(IMGUI_BACKEND_RENDERER "opengl3" CACHE STRING
    "Renderer backend(s), semicolon-separated. Allowed: none null allegro5 dx9 dx10 dx11 dx12 metal opengl2 opengl3 sdlrenderer2 sdlrenderer3 vulkan wgpu")
set(IMGUI_BACKEND_PLATFORM "glfw" CACHE STRING
    "Platform backend(s), semicolon-separated. Allowed: none null allegro5 android glfw glut osx sdl2 sdl3 win32")

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
foreach(_imgui_renderer IN LISTS IMGUI_BACKEND_RENDERER)
    if(_imgui_renderer STREQUAL "none")
        # do nothing. imgui will compile without a renderer.
    elseif(_imgui_renderer STREQUAL "null")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_null.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_null.cpp")
        set(_imgui_added_null TRUE)
    elseif(_imgui_renderer STREQUAL "allegro5")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_allegro5.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_allegro5.cpp")
        set(_imgui_added_allegro5 TRUE)
        # untested, needs work
    elseif(_imgui_renderer STREQUAL "dx9")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.cpp")
        target_link_libraries(imgui PRIVATE d3d9)
    elseif(_imgui_renderer STREQUAL "dx10")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_dx10.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_dx10.cpp")
        target_link_libraries(imgui PRIVATE d3d10)
    elseif(_imgui_renderer STREQUAL "dx11")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp")
        target_link_libraries(imgui PRIVATE d3d11)
    elseif(_imgui_renderer STREQUAL "dx12")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp")
        target_link_libraries(imgui PRIVATE d3d12 dxgi)
    elseif(_imgui_renderer STREQUAL "metal")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_metal.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_metal.mm")
        target_link_libraries(imgui PRIVATE "-framework Metal -framework QuartzCore")
    elseif(_imgui_renderer STREQUAL "opengl2")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp")

        find_package(OpenGL REQUIRED)
        if(TARGET OpenGL::OpenGL)
            target_link_libraries(imgui PRIVATE OpenGL::OpenGL)
        elseif(TARGET OpenGL::GL)
            target_link_libraries(imgui PRIVATE OpenGL::GL)
        endif()
    elseif(_imgui_renderer STREQUAL "opengl3")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")

        find_package(OpenGL REQUIRED)
        if(TARGET OpenGL::OpenGL)
            target_link_libraries(imgui PRIVATE OpenGL::OpenGL)
        elseif(TARGET OpenGL::GL)
            target_link_libraries(imgui PRIVATE OpenGL::GL)
        endif()
    elseif(_imgui_renderer STREQUAL "sdlrenderer2")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp")
        # untested, needs work
    elseif(_imgui_renderer STREQUAL "sdlrenderer3")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp")
        # untested, needs work
    elseif(_imgui_renderer STREQUAL "vulkan")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp")
        # This likely requires you to install the Vulkan SDK.
        find_package(Vulkan REQUIRED)
        target_link_libraries(imgui PRIVATE Vulkan::Vulkan)
        # Untested.
    elseif(_imgui_renderer STREQUAL "wgpu")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_wgpu.cpp")
        # untested, needs work
    else()
        message(FATAL_ERROR "IMGUI_BACKEND_RENDERER contains unknown value '${_imgui_renderer}'")
    endif()
endforeach()

foreach(_imgui_platform IN LISTS IMGUI_BACKEND_PLATFORM)
    if(_imgui_platform STREQUAL "none")
        # do nothing. imgui will compile without a platform.
    elseif(_imgui_platform STREQUAL "null" AND _imgui_added_null)
        # already included by the renderer backend above
    elseif(_imgui_platform STREQUAL "allegro5" AND _imgui_added_allegro5)
        # already included by the renderer backend above
    elseif(_imgui_platform STREQUAL "android")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_android.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_android.cpp")
        # untested, needs work
    elseif(_imgui_platform STREQUAL "glfw")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")

        if(NOT TARGET glfw)
            include(FetchContent)
            FetchContent_Declare(
                glfw
                GIT_REPOSITORY https://github.com/glfw/glfw.git
                GIT_TAG tags/3.4
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
    elseif(_imgui_platform STREQUAL "glut")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_glut.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_glut.cpp")
        # untested, needs work
    elseif(_imgui_platform STREQUAL "osx")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_osx.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_osx.mm")
        target_link_libraries(imgui PRIVATE "-framework AppKit -framework MetalKit -framework Foundation -framework GameController")
        # This builds successfully, but hangs on startup. Needs work.
    elseif(_imgui_platform STREQUAL "sdl2")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp")
        # untested, needs work
    elseif(_imgui_platform STREQUAL "sdl3")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp")
        # untested, needs work
    elseif(_imgui_platform STREQUAL "win32")
        target_sources(imgui PRIVATE "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h" "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp")
    else()
        message(FATAL_ERROR "IMGUI_BACKEND_PLATFORM contains unknown value '${_imgui_platform}'")
    endif()
endforeach()

if(IMGUI_BUILD_EXAMPLES)
    set(_imgui_any_example FALSE)
    foreach(_imgui_renderer IN LISTS IMGUI_BACKEND_RENDERER)
        foreach(_imgui_platform IN LISTS IMGUI_BACKEND_PLATFORM)
            set(_imgui_example_name "")
            set(_imgui_example_src "")
            if(_imgui_renderer STREQUAL "none" AND _imgui_platform STREQUAL "none")
                set(_imgui_example_name "imgui_example_null")
                set(_imgui_example_src "examples/example_null/main.cpp")
            elseif(_imgui_renderer STREQUAL "opengl2" AND _imgui_platform STREQUAL "glfw")
                set(_imgui_example_name "imgui_example_glfw_opengl2")
                set(_imgui_example_src "examples/example_glfw_opengl2/main.cpp")
            elseif(_imgui_renderer STREQUAL "opengl3" AND _imgui_platform STREQUAL "glfw")
                set(_imgui_example_name "imgui_example_glfw_opengl3")
                set(_imgui_example_src "examples/example_glfw_opengl3/main.cpp")
            elseif(_imgui_renderer STREQUAL "vulkan" AND _imgui_platform STREQUAL "glfw")
                set(_imgui_example_name "imgui_example_glfw_vulkan")
                set(_imgui_example_src "examples/example_glfw_vulkan/main.cpp")
            elseif(_imgui_renderer STREQUAL "dx9" AND _imgui_platform STREQUAL "win32")
                set(_imgui_example_name "imgui_example_win32_directx9")
                set(_imgui_example_src "examples/example_win32_directx9/main.cpp")
            elseif(_imgui_renderer STREQUAL "dx10" AND _imgui_platform STREQUAL "win32")
                set(_imgui_example_name "imgui_example_win32_directx10")
                set(_imgui_example_src "examples/example_win32_directx10/main.cpp")
            elseif(_imgui_renderer STREQUAL "dx11" AND _imgui_platform STREQUAL "win32")
                set(_imgui_example_name "imgui_example_win32_directx11")
                set(_imgui_example_src "examples/example_win32_directx11/main.cpp")
            elseif(_imgui_renderer STREQUAL "dx12" AND _imgui_platform STREQUAL "win32")
                set(_imgui_example_name "imgui_example_win32_directx12")
                set(_imgui_example_src "examples/example_win32_directx12/main.cpp")
            elseif(_imgui_renderer STREQUAL "opengl3" AND _imgui_platform STREQUAL "win32")
                set(_imgui_example_name "imgui_example_win32_opengl3")
                set(_imgui_example_src "examples/example_win32_opengl3/main.cpp")
            elseif(_imgui_renderer STREQUAL "metal" AND _imgui_platform STREQUAL "glfw")
                set(_imgui_example_name "imgui_example_glfw_metal")
                set(_imgui_example_src "examples/example_glfw_metal/main.mm")
            elseif(_imgui_renderer STREQUAL "metal" AND _imgui_platform STREQUAL "osx")
                set(_imgui_example_name "imgui_example_apple_metal")
                set(_imgui_example_src "examples/example_apple_metal/main.mm")
            elseif(_imgui_renderer STREQUAL "opengl2" AND _imgui_platform STREQUAL "osx")
                set(_imgui_example_name "imgui_example_apple_opengl2")
                set(_imgui_example_src "examples/example_apple_opengl2/main.mm")
            endif()

            if(_imgui_example_name)
                add_executable(${_imgui_example_name} "${_imgui_example_src}")
                target_link_libraries(${_imgui_example_name} PRIVATE imgui)
                target_include_directories(${_imgui_example_name} PRIVATE "backends/")
                set(_imgui_any_example TRUE)
            endif()
        endforeach()
    endforeach()

    if(NOT _imgui_any_example)
        message(WARNING "IMGUI_BUILD_EXAMPLES is set, but no example was built. \nNo supported combination of renderer and platform was found in IMGUI_BACKEND_RENDERER='${IMGUI_BACKEND_RENDERER}' / IMGUI_BACKEND_PLATFORM='${IMGUI_BACKEND_PLATFORM}'. \nSet IMGUI_BUILD_EXAMPLES=OFF to disable this warning.")
    endif()
endif()
