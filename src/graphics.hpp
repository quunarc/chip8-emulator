#pragma once

#include <SDL3/SDL.h>

// Handles all the graphics related stuff and tries to be decoupled from the Chip8 class
namespace Graphics
{

void InitSDL();
void PollEvents(bool& running);
void Render();
void Cleanup();

inline SDL_Window* window = nullptr;
inline SDL_Renderer* renderer = nullptr;

inline SDL_Event event;

}
