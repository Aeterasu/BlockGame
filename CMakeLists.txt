cmake_minimum_required(VERSION 3.24)
project(game LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# dependencies 

if(NOT EMSCRIPTEN)
    include(FetchContent)

    set(SDL_SHARED       OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC       ON  CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-3.4.18
        GIT_SHALLOW    TRUE
        FIND_PACKAGE_ARGS 3.4.18 CONFIG
    )

    FetchContent_MakeAvailable(SDL3)

endif()

add_library(glad STATIC vendor/glad/src/gles2.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# game
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.c)

add_executable(game ${GAME_SOURCES})

target_include_directories(game PRIVATE src)
target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(game PRIVATE glad)

if(EMSCRIPTEN)
    target_compile_options(game PRIVATE --use-port=sdl3)
    target_link_options(game PRIVATE --use-port=sdl3)
    set_target_properties(game PROPERTIES SUFFIX ".html")
else()
    target_link_libraries(game PRIVATE SDL3::SDL3)
endif()
