cmake_minimum_required(VERSION 3.20)
project(game_sdl2_c C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

file(GLOB SRC src/*.c)

add_executable(game ${SRC}
        src/highscore.c)

target_include_directories(game PRIVATE
        include
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(game PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        m
)

target_compile_options(game PRIVATE
        -Wall -Wextra -g
)
