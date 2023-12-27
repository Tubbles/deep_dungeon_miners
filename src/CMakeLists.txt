if(MSVC)
    target_compile_options(${TARGET} PRIVATE /W4 /WX)
else()
    add_definitions(-Wall -Wextra -Wpedantic -Werror)
endif()

# Add some sources
set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/collision.cpp
    ${CMAKE_CURRENT_LIST_DIR}/event.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gamepad.cpp
    ${CMAKE_CURRENT_LIST_DIR}/log.cpp
)

# Add debug sources
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/debug.cpp
)
endif()

target_sources(${TARGET} PRIVATE ${SOURCES})

set_source_files_properties(
    ${SOURCES}
    DIRECTORY ${CMAKE_SOURCE_DIR}
    PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wpedantic -Werror"
)
