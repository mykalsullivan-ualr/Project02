#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <memory>  // For std::unique_ptr
#include "Input.h"
#include "GameObject.h"

class Engine
{
private:
    static bool m_IsRunning;                               // Engine running state (static)
    static SDL_Window* m_Window;                           // SDL m_Window (static)
    static SDL_Renderer* m_Renderer;                       // SDL m_Renderer (static)
    static std::vector<std::unique_ptr<GameObject>> m_GameObjects;  // Track game objects
    
public:
    static int width;

    // Initialize the Engine (static)
    static bool init(
        const char *title,
        const int width,
        const int height)
    {
        Engine::width = width;
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
            return false;
        }

        m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
        if (!m_Window)
        {
            SDL_Log("Failed to create m_Window: %s", SDL_GetError());
            return false;
        }

        m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
        if (!m_Renderer)
        {
            SDL_Log("Failed to create m_Renderer: %s", SDL_GetError());
            return false;
        }

        m_IsRunning = true;
        return true;
    }

    // Handle events (static)
    static void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                m_IsRunning = false;
            Input::processEvent(event);  // Call static Input method
        }
    }

    // Update all game objects (static)
    static void update()
    {
        for (auto& gameObject : m_GameObjects)
            gameObject->update();  // Update each GameObject
    }

    // Render all game objects (static)
    static void render() {
        SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_Renderer);

        for (auto& gameObject : m_GameObjects)
            gameObject->draw();  // Draw each GameObject

        SDL_RenderPresent(m_Renderer);
    }

    // Clean up SDL resources (static)
    static void clean()
    {
        if (m_Renderer)
            SDL_DestroyRenderer(m_Renderer);
        
        if (m_Window)
            SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }

    // Check if the engine is still running (static)
    static bool running()
    {
        return m_IsRunning;
    }

    // Add a GameObject to the engine (static)
    static void addGameObject(std::unique_ptr<GameObject> gameObject) {
        m_GameObjects.push_back(std::move(gameObject));  // Add the game object to the list
    }

    // Run the engine (static)
    static void run()
    {
        while (m_IsRunning)
        {
            handleEvents();
            update();
            render();
            SDL_Delay(16);
        }

        clean();
    }

    static SDL_Renderer* getRenderer()
    {
        return m_Renderer;
    }
};
