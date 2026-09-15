#include <filesystem.hpp>
#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "cpu.hpp"

int main()
{
    CHIP8 emu;

    emu.Start();
    emu.LoadToMemory("../roms/helloworld.rom");

    std::cout << (int)emu._memory[512];

    return 0;
}
