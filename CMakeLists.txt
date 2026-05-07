cmake_minimum_required(VERSION 3.20)
project(game_sdl2_c C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED IMPORTED_TARGET SDL2_mixer)

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
        src/title.c
        src/player.c
        include/player.h
)

target_include_directories(game PRIVATE include)

target_link_libraries(game PRIVATE
        PkgConfig::SDL2
        PkgConfig::SDL2_IMAGE
        PkgConfig::SDL2_MIXER
        m
)

target_compile_options(game PRIVATE
        -Wall -Wextra -g
)