#include <SDL3/SDL.h>
#include <iostream>
#include <memory>

#include "cpu.hpp"

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "SDL3Test",
        640, 480,
        0
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 32, 32, 32, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // Pump events for 3 seconds so the window is responsive.
    // A plain SDL_Delay() blocks the event loop and some window
    // managers won't actually show the window.
    bool running = true;
    Uint64 start = SDL_GetTicks();
    SDL_Event event;

    // Now do your CHIP8 stuff
    std::unique_ptr<CHIP8> C8 = std::make_unique<CHIP8>();

    C8->Start();
    C8->LoadToMemory("../roms/helloworld.rom");

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        C8->Cycle();

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    std::cout << (int)C8->GetMemory(0x200) << std::endl;
    std::cout << (int)C8->GetRegister(1) << std::endl;
    std::cout << (int)C8->GetRegister(3) << std::endl;
    std::cout << (int)C8->GetRegister(4) << std::endl;

    // Cleanup — renderer must be destroyed before the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
