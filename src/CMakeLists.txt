# if(NOT MSVC)
#     target_compile_options(${TARGET} PRIVATE
#         -pedantic
#         -Wall
#         -Wcast-align
#         -Wconversion
#         -Wdouble-promotion
#         -Wduplicated-branches
#         -Wduplicated-cond
#         -Wextra
#         -Wformat=2
#         -Wimplicit-fallthrough
#         -Wlogical-op
#         -Wmisleading-indentation
#         -Wnon-virtual-dtor
#         -Wnull-dereference
#         -Wold-style-cast
#         -Woverloaded-virtual
#         -Wpedantic
#         -Wshadow
#         -Wsign-conversion
#         -Wunused
#         -Wuseless-cast
#         # -Weffc++
#     )
# else()
#     message(FATAL_ERROR "Windows not supported yet")
# endif()

# Add some sources
set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/collision.cpp
    ${CMAKE_CURRENT_LIST_DIR}/event.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gamepad.cpp
    ${CMAKE_CURRENT_LIST_DIR}/log.cpp
    ${CMAKE_CURRENT_LIST_DIR}/timer.cpp
)

# Add debug sources
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    list(APPEND SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/debug.cpp
    )
endif()


set_source_files_properties(
    ${SOURCES}
    DIRECTORY ${CMAKE_SOURCE_DIR}
    PROPERTIES COMPILE_FLAGS "${WARN_FLAGS}"
)

target_sources(${TARGET} PRIVATE ${SOURCES})
