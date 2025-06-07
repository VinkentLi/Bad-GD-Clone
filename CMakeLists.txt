cmake_minimum_required(VERSION 3.16)
project(GDClone)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /Od /Zi)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
    endif()
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /O2)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O3 -flto)
        target_link_options(${PROJECT_NAME} PRIVATE -flto)
    endif()
endif()

if(EMSCRIPTEN)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_IMAGE=2
        -sSDL2_IMAGE_FORMATS=["png"]
        -sUSE_SDL_MIXER=2
        -sSDL2_MIXER_FORMATS=["ogg"]
        -sUSE_SDL_TTF=2
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        -sUSE_SDL=2
        -sUSE_SDL_IMAGE=2
        -sSDL2_IMAGE_FORMATS=["png"]
        -sUSE_SDL_MIXER=2
        -sSDL2_MIXER_FORMATS=["ogg"]
        -sUSE_SDL_TTF=2
        -sWASM=1
        -sENVIRONMENT=web
        -sALLOW_MEMORY_GROWTH=1
        --bind
        --preload-file ../res
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_options(${PROJECT_NAME} PRIVATE -gsource-map -O0 -sASSERTIONS=2)
    else()
        target_link_options(${PROJECT_NAME} PRIVATE -O3 -flto -sASSERTIONS=0 -sEXIT_RUNTIME=0)
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".js")

else()
    include(FetchContent)

    find_package(SDL2 CONFIG QUIET)
    if(NOT SDL2_FOUND)
        message(STATUS "SDL2 not found, using FetchContent")
        FetchContent_Declare(
            SDL2
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-2.32.8
        )
        FetchContent_MakeAvailable(SDL2)
    endif()

    find_package(SDL2_image CONFIG QUIET)
    if(NOT SDL2_image_FOUND)
        message(STATUS "SDL2_image not found, using FetchContent")

        # vendored so sdl downloads dependencies for us
        set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
        set(SDL2IMAGE_PNG ON CACHE BOOL "" FORCE)
        # disable everything that we don't need
        set(SDL2IMAGE_JXL OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_TIF OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_WEBP OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_AVIF OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_BMP OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_GIF OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_JPG OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_LBM OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_PCX OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_PNM OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_QOI OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_SVG OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_TGA OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_XCF OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_XPM OFF CACHE BOOL "" FORCE)
        set(SDL2IMAGE_XV OFF CACHE BOOL "" FORCE)

        FetchContent_Declare(
            SDL2_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-2.8.8
        )
        FetchContent_MakeAvailable(SDL2_image)
    endif()

    find_package(SDL2_mixer CONFIG QUIET)
    if(NOT SDL2_mixer_FOUND)
        message(STATUS "SDL2_mixer not found, using FetchContent")

        set(SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE)
        set(SDL2MIXER_OGG ON CACHE BOOL "" FORCE)
        set(SDL2MIXER_MP3 OFF CACHE BOOL "" FORCE)
        set(SDL2MIXER_MIDI OFF CACHE BOOL "" FORCE)
        set(SDL2MIXER_MOD OFF CACHE BOOL "" FORCE)
        set(SDL2MIXER_OPUS OFF CACHE BOOL "" FORCE)
        set(SDL2MIXER_WAVE OFF CACHE BOOL "" FORCE)

        FetchContent_Declare(
            SDL2_mixer
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
            GIT_TAG release-2.8.1
        )
        FetchContent_MakeAvailable(SDL2_mixer)
    endif()

    find_package(SDL2_ttf CONFIG QUIET)
    if(NOT SDL2_ttf_FOUND)
        message(STATUS "SDL2_ttf not found, using FetchContent")

        set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)

        FetchContent_Declare(
            SDL2_ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
            GIT_TAG release-2.24.0
        )
        FetchContent_MakeAvailable(SDL2_ttf)
    endif()

    if(MINGW)
        target_link_libraries(${PROJECT_NAME} PRIVATE -lmingw32)
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
        else()
            target_link_options(${PROJECT_NAME} PRIVATE -mconsole)
        endif()
    elseif(MSVC)
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
        else()
            set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
        endif()
    endif()

    if(TARGET SDL2::SDL2main)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
    endif()

    target_link_libraries(${PROJECT_NAME}
        PRIVATE SDL2::SDL2
                SDL2_image::SDL2_image
                SDL2_mixer::SDL2_mixer
                SDL2_ttf::SDL2_ttf
    )

    if(WIN32)
        # https://discourse.cmake.org/t/how-to-copy-dlls-to-binary-directory/10154
        add_custom_command (
            TARGET "${PROJECT_NAME}" POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                    "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>" USES_TERMINAL COMMAND_EXPAND_LISTS
        )
    endif()
endif()
