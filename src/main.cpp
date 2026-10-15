#include <SDL3/SDL.h>
#include <iostream>
#include <memory>

#include "cpu.hpp"
#include "graphics.hpp"

int main()
{
    std::unique_ptr<CHIP8> C8 = std::make_unique<CHIP8>();

    Graphics::InitSDL();
    bool running = true;

    C8->Start();
    C8->LoadToMemory("../roms/helloworld.rom");

    while (running)
    {
        Graphics::PollEvents(running);
        // for (int i{0}; i<=50; i++) C8->Cycle();
        C8->Cycle();
        Graphics::Render();
    }

    std::cout << (int)C8->GetMemory(0x200) << std::endl;
    std::cout << (int)C8->GetRegister(1) << std::endl;
    std::cout << (int)C8->GetRegister(3) << std::endl;
    std::cout << (int)C8->GetRegister(4) << std::endl;

    Graphics::Cleanup();

    return 0;
}
