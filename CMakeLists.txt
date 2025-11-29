cmake_minimum_required(VERSION 3.15)
project(space_invaders C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

set(SOURCES
    src/main.c
    src/game.c
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
    message(STATUS "Building for Web (WASM) with Emscripten")
else()
    message(STATUS "Building for Native platform")
endif()

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

add_subdirectory(raylib)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} raylib)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    target_link_options(${PROJECT_NAME} PRIVATE
        -sUSE_GLFW=3
        -sUSE_WEBGL2=1
        -sASYNCIFY
        -sALLOW_MEMORY_GROWTH=1
        --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets
        -O3
    )
endif()
