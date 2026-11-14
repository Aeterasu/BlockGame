cmake_minimum_required(VERSION 3.24)
project(game LANGUAGES C)

set(GAME_NAME "BlockGame" CACHE STRING "Game name")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# dependencies

include(FetchContent)

if(NOT EMSCRIPTEN)
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
	set(SDL_SHARED       OFF CACHE BOOL "" FORCE)
	set(SDL_STATIC       ON  CACHE BOOL "" FORCE)
	set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

	# SDL3
	FetchContent_Declare(
		SDL3
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG        release-3.4.18
		GIT_SHALLOW    TRUE
		FIND_PACKAGE_ARGS 3.4.18 CONFIG
	)

	FetchContent_MakeAvailable(SDL3)
else()
	# Emscripten has an SDL3 port but no SDL3_image port, so SDL3_image is
	# built from source below. Provide the targets its CMake looks for,
	# backed by the SDL3 port.
	add_library(SDL3::Headers INTERFACE IMPORTED)
	add_library(SDL3::SDL3 INTERFACE IMPORTED)
	target_compile_options(SDL3::SDL3 INTERFACE --use-port=sdl3)
	target_link_options(SDL3::SDL3 INTERFACE --use-port=sdl3)
endif()

set(SDLIMAGE_VENDORED    ON  CACHE BOOL "" FORCE)
set(SDLIMAGE_DEPS_SHARED OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_PNG_SHARED  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_SAMPLES     OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TESTS       OFF CACHE BOOL "" FORCE)
foreach(_fmt ANI AVIF BMP GIF JPG JXL LBM PCX PNM QOI SVG TGA TIF WEBP XCF XPM XV)
	set(SDLIMAGE_${_fmt} OFF CACHE BOOL "" FORCE)
endforeach()
set(SDLIMAGE_PNG ON CACHE BOOL "" FORCE)

FetchContent_Declare(
	SDL3_image
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG        release-3.4.8
	GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(SDL3_image)

add_library(glad STATIC vendor/glad/src/gles2.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# game
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.c)

add_executable(game ${GAME_SOURCES})

target_include_directories(game PRIVATE src)
target_compile_definitions(game PRIVATE GAME_NAME="${GAME_NAME}")
target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(game PRIVATE glad SDL3::SDL3 SDL3_image::SDL3_image)

if(EMSCRIPTEN)
	target_compile_options(game PRIVATE --use-port=sdl3)
	target_link_options(game PRIVATE
		--use-port=sdl3
		--preload-file ${CMAKE_SOURCE_DIR}/asset@asset
		--shell-file=${CMAKE_SOURCE_DIR}/index.html
	)
	set_target_properties(game PROPERTIES OUTPUT_NAME "index" SUFFIX ".html")
	set_property(TARGET game APPEND PROPERTY LINK_DEPENDS "${CMAKE_SOURCE_DIR}/index.html")
endif()
