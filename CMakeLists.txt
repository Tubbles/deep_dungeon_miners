cmake_minimum_required(VERSION 3.25.1)
project(deep_dungeon_miners)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Set up the main target
set(TARGET "${PROJECT_NAME}")
add_executable(${TARGET})
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 20)

# Add various project settings
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_definitions(DEBUG)
endif()

if(NOT MSVC)
    set(WARN_FLAGS "-pedantic -Wall -Wcast-align -Wconversion -Wdouble-promotion -Wduplicated-branches -Wduplicated-cond -Wextra -Wformat=2 -Wimplicit-fallthrough -Wlogical-op -Wmisleading-indentation -Wnon-virtual-dtor -Wnull-dereference -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wshadow -Wsign-conversion -Wunused -Wuseless-cast -Weffc++")
else()
    message(FATAL_ERROR "Windows not supported yet")
endif()

# Set version information
set(${TARGET}_VERSION_MAJOR 1)
set(${TARGET}_VERSION_MINOR 0)

# Include modules
add_subdirectory(modules)
add_subdirectory(cmake_modules)

# Detect and add SFML
find_package(SFML 2.5 REQUIRED system window graphics network audio)
find_package(fmt)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET} ${SFML_LIBRARIES} fmt::fmt OpenGL::GL)

# Add the sources
add_subdirectory(src)

# Install target
install(TARGETS ${TARGET} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR "${${TARGET}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${TARGET}_VERSION_MINOR}")
include(CPack)
