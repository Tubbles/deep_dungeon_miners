cmake_minimum_required(VERSION 3.23)

set(OpenGL_GL_PREFERENCE GLVND)

# Set the project name
project(my_project_name)

# Packages
include(FetchContent)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(fmt)

# SDL2
find_package(SDL2)

if (SDL2_FOUND)
    if (NOT TARGET glad AND EXPECTED_COMPILER)
        FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG df8e9e16110b305479a875399cee13daa0ccadd9)
        FetchContent_MakeAvailable(glad)
    endif ()
    if (SDL2_FOUND AND EXPECTED_COMPILER)

    endif ()
endif ()

# ImGUI
find_package(OpenGL)

FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG df8e9e16110b305479a875399cee13daa0ccadd9)
FetchContent_MakeAvailable(glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.2)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.77
)
FetchContent_GetProperties(imgui)

if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR}/)
    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp)
    file(GLOB IMGUI_HEADERS ${imgui_SOURCE_DIR}/*.h)
    add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_SOURCES} ${imgui_SOURCE_DIR}/examples/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/examples/imgui_impl_opengl3.cpp)
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

    target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})
    target_link_libraries(imgui ${OPENGL_LIBRARIES} glfw glad)
endif ()


# Add the executable
add_executable(deep_dungeon_miners main.cpp)

set_target_properties(deep_dungeon_miners PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED true
    COMPILE_DEFINITIONS IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_include_directories(deep_dungeon_miners
    PUBLIC
    ${IMGUI_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    PRIVATE
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(deep_dungeon_miners PUBLIC
    fmt::fmt
    ${SDL2_LIBRARIES}
    glad
    ${OPENGL_LIBRARIES}
    glfw
    imgui
)
