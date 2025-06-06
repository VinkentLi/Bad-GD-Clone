cmake_minimum_required(VERSION 3.7)
project(GDClone)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE HEADER_FILES ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose build type (Debug or Release): ")

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wno-c++11-narrowing)

if (WIN32 AND NOT EMSCRIPTEN AND CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -flto -DNDEBUG)
endif()

if(EMSCRIPTEN)
    target_compile_options(${PROJECT_NAME} PRIVATE 
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sSDL2_IMAGE_FORMATS=[\"png\"]"
        "-sUSE_SDL_MIXER=2"
        "-sSDL2_MIXER_FORMATS=[\"wav\", \"ogg\"]"
        "-sUSE_SDL_TTF=2"
    )
    target_link_options(${PROJECT_NAME} PRIVATE 
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sSDL2_IMAGE_FORMATS=[\"png\"]"
        "-sUSE_SDL_MIXER=2"
        "-sSDL2_MIXER_FORMATS=[\"wav\", \"ogg\"]"
        "-sUSE_SDL_TTF=2"
        "-sWASM=1"
        "-sENVIRONMENT=web"
        "-sALLOW_MEMORY_GROWTH=1"
        "--bind"
        "--preload-file" "../res"
    )

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_options(${PROJECT_NAME} PRIVATE "-gsource-map" "-O0" "-sASSERTIONS=2")
    else()
        target_link_options(${PROJECT_NAME} PRIVATE
            "-O3"
            "-flto"
            "-sASSERTIONS=0"
            "-sEXIT_RUNTIME=0"
        )
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".js")

else()
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2 SDL2main)
    find_package(SDL2_image REQUIRED CONFIG COMPONENTS SDL2_image)
    find_package(SDL2_mixer REQUIRED CONFIG COMPONENTS SDL2_mixer)
    find_package(SDL2_ttf REQUIRED CONFIG COMPONENTS SDL2_ttf)

    if (MINGW)
        target_link_libraries(${PROJECT_NAME} PRIVATE -lmingw32)
    endif()

    if (TARGET SDL2::SDL2main)
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
