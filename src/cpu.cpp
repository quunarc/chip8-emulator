#include "cpu.hpp"

void CHIP8::Start()
{
    // Clear Memory
    for (int i{0}; i < 4096; i++) _memory[i] = 0;

    // Set Registers
    _r_PC = 0x200;
}
