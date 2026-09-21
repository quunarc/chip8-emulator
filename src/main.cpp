#include <iostream>
#include <memory>

#include "cpu.hpp"

int main()
{
    // its a good practice to initialize large classes on the heap
    // even though this is just a few thousand bytes
    std::unique_ptr<CHIP8> C8 = std::make_unique<CHIP8>();

    C8->Start();
    C8->LoadToMemory("../roms/helloworld.rom");
    C8->FetchDecodeExecute();

    std::cout << (int)C8->_memory[0x200] << std::endl;
    std::cout << (int)C8->_registers[2];

    return 0;
}
