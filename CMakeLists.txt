cmake_minimum_required(VERSION 3.20)
project(BlockGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic
)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    # USE_SDL / USE_SDL_IMAGE need to be on the COMPILE line too - they
    # control which headers Emscripten's SDL2/SDL2_image ports expose.
    # USE_WEBGL2 / FULL_ES3 are link-only settings; putting them here as
    # well just produces "linker setting ignored during compilation"
    # warnings, so they're kept out of this list.
    target_compile_options(${PROJECT_NAME} PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_IMAGE=2
				-sUSE_SDL_TTF=2
    )

    set(SHELL_FILE ${CMAKE_SOURCE_DIR}/web/shell.html)

    target_link_options(${PROJECT_NAME} PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_IMAGE=2
				-sUSE_SDL_TTF=2
        -sSDL2_IMAGE_FORMATS=["png"]
        -sALLOW_MEMORY_GROWTH=1
        -sASYNCIFY
        -sNO_EXIT_RUNTIME=1
        -sUSE_WEBGL2=1
        -sMIN_WEBGL_VERSION=2
        -sMAX_WEBGL_VERSION=2
        -sFULL_ES3=1
        -sMODULARIZE=0
        --shell-file ${SHELL_FILE}
        $<$<TARGET_EXISTS:${PROJECT_NAME}>:>
    )
    if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
        target_link_options(${PROJECT_NAME} PRIVATE
            --preload-file ${CMAKE_SOURCE_DIR}/assets@assets
        )
    endif()

else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
		find_package(SDL2_ttf REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

    if(TARGET SDL2::SDL2)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
        if(TARGET SDL2::SDL2main)
            target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
        endif()
    else()
        # fallback for older distro SDL2 config that only sets variables
        target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
    endif()

    if(TARGET SDL2_image::SDL2_image)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_LIBRARIES})
    endif()

		if(TARGET SDL2_ttf::SDL2_ttf)
			target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)
		else()
			target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_TTF_LIBRARIES})
		endif()

    add_library(glad STATIC external/glad/src/gl.c)
    target_include_directories(glad PUBLIC external/glad/include)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()
