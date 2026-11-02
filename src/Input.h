#pragma once
#include <SDL2/SDL.h>
#include <unordered_map>
#include <set>
#include <utility>  // for std::pair

class Input
{
    static std::set<SDL_Keycode> m_KeysDown;               // Track which keys are down (static)
    static std::set<Uint8> m_MouseButtonsDown;             // Track which mouse buttons are down (static)
    static std::pair<int, int> m_MousePosition;            // Store the current mouse position (static)

public:
    // Update the input state based on SDL events (static method)
    static void processEvent(const SDL_Event &event)
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            m_KeysDown.insert(event.key.keysym.sym);
            break;

        case SDL_KEYUP:
            m_KeysDown.erase(event.key.keysym.sym);
            break;

        case SDL_MOUSEBUTTONDOWN:
            m_MouseButtonsDown.insert(event.button.button);
            break;

        case SDL_MOUSEBUTTONUP:
            m_MouseButtonsDown.erase(event.button.button);
            break;

        case SDL_MOUSEMOTION:
            m_MousePosition = { event.motion.x, event.motion.y };
            break;
        default: break;
        }
    }

    // Check if a specific key is currently down (static method)
    static bool isKeyDown(const SDL_Keycode key)
    {
        return m_KeysDown.find(key) != m_KeysDown.end();
    }

    // Check if a specific mouse button is currently down (static method)
    static bool isMouseButtonDown(Uint8 button)
    {
        return m_MouseButtonsDown.find(button) != m_MouseButtonsDown.end();
    }

    // Get the current mouse position (static method)
    static std::pair<int, int> getMousePosition()
    {
        return m_MousePosition;
    }
};
