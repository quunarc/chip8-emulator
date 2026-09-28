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
    C8->Fetch_Decode_Execute();

    std::cout << (int)C8->GetMemory(0x200) << std::endl;
    std::cout << (int)C8->GetRegister(1) << std::endl;
    std::cout << (int)C8->GetRegister(3) << std::endl;
    std::cout << (int)C8->GetRegister(4) << std::endl;

    return 0;
}
