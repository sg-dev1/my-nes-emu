cmake_minimum_required(VERSION 3.16)
project(MyNesEmu)

set(CMAKE_CXX_STANDARD 23)

add_executable(my_nes_emu
    src/main.cpp
    src/bus.cpp
    src/cpu.cpp
    src/ppu.cpp
    src/mapper0.cpp
    src/cartridge.cpp
    src/renderer.cpp
)

target_include_directories(my_nes_emu PRIVATE src)

# -------------------------
# SDL2 via pkg-config
# Installation in Linux Mint 22.3 e.g. with
#   sudo apt install libsdl2-dev
# -------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

target_include_directories(my_nes_emu PRIVATE
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(my_nes_emu PRIVATE
    ${SDL2_LIBRARIES}
)

target_compile_options(my_nes_emu PRIVATE
    ${SDL2_CFLAGS_OTHER}
)

# -------------------------
# Warnings
# -------------------------
target_compile_options(my_nes_emu PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion>
)