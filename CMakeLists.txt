cmake_minimum_required(VERSION 3.15)
project(space-invaders)

find_package(raylib 3.0 REQUIRED)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} raylib)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
