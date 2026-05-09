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
)

target_include_directories(my_nes_emu PRIVATE src)

target_compile_options(my_nes_emu PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion>
)