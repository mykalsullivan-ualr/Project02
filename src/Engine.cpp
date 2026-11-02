#include "Engine.h"

// Definition of static members
bool Engine::m_IsRunning = false;
SDL_Window* Engine::m_Window = nullptr;
SDL_Renderer* Engine::m_Renderer = nullptr;
std::vector<std::unique_ptr<GameObject>> Engine::m_GameObjects;
int Engine::width = 0;