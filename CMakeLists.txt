cmake_minimum_required(VERSION 3.7)
project(GDClone)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE HEADER_FILES ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose build type (Debug or Release): ")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -g -O0)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -O2)
endif()

# Emscripten-specific setup
if(EMSCRIPTEN)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sSDL2_IMAGE_FORMATS=[\"png\"]"
        "-sUSE_SDL_MIXER=2"
        "-sSDL2_MIXER_FORMATS=[\"wav\"]"
        "-sUSE_SDL_TTF=2"
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        "--bind"
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sSDL2_IMAGE_FORMATS=[\"png\"]"
        "-sUSE_SDL_MIXER=2"
        "-sSDL2_MIXER_FORMATS=[\"wav\"]"
        "-sUSE_SDL_TTF=2"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sEXIT_RUNTIME=1"
        "--preload-file"
        "../res"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".js")
else()
    # Native build
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2 SDL2main)
    find_package(SDL2_image REQUIRED CONFIG COMPONENTS SDL2_image)
    find_package(SDL2_mixer REQUIRED CONFIG COMPONENTS SDL2_mixer)
    find_package(SDL2_ttf REQUIRED CONFIG COMPONENTS SDL2_ttf)

    if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
        target_link_libraries(${PROJECT_NAME} PRIVATE -lmingw32)
    endif()

    if (TARGET SDL2::SDL2main)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
    endif()

    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)
endif()