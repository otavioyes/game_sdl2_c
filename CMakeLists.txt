cmake_minimum_required(VERSION 3.20)
project(game_sdl2_c C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

add_executable(game
        src/background.c
        src/draw.c
        src/highscore.c
        src/init.c
        src/input.c
        src/main.c
        src/sound.c
        src/stage.c
        src/util.c
        src/text.c
)


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