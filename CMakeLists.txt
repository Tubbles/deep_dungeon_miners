cmake_minimum_required(VERSION 3.25.1)
project(deep_dungeon_miners)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Set up the main target
set(TARGET "${PROJECT_NAME}")
add_executable(${TARGET})
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 20)

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
