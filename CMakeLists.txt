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
        # WebGL & Raylib
        -sUSE_GLFW=3
        -sGL_ENABLE_GET_PROC_ADDRESS=1
        -sUSE_WEBGL2=1

        # Performance
        -O3
        -flto
        -sALLOW_MEMORY_GROWTH=1
        -sASYNCIFY

        # Disable debug UI / outline
        -g0
        -sASSERTIONS=0
        -sSAFE_HEAP=0
        -sDEMANGLE_SUPPORT=0
        -sDISABLE_EXCEPTION_CATCHING=1

        # Use relative paths for GitHub Pages
        -sENVIRONMENT=web
        -sRELOCATABLE=0
        -sMODULARIZE=0

        # Asset system
        --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets
    )
endif()
