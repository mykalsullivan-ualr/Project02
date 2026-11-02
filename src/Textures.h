#pragma once
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <memory>

class Textures
{
    // Custom deleter for SDL_Texture to handle SDL_DestroyTexture automatically
    struct SDL_Deleter
    {
        void operator()(SDL_Texture* texture) const
        {
            if (texture)
                SDL_DestroyTexture(texture);
        }
    };

    // Static map to store m_Textures with custom keys and unique_ptrs for automatic cleanup
    static std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDL_Deleter>> m_Textures;
    
public:
    // Load a texture from a file and store it with the given key
    static bool load(
        const std::string& key,
        const std::string& filePath,
        SDL_Renderer *renderer)
    {
        SDL_Surface *tempSurface = IMG_Load(filePath.c_str());  // Using BMP for simplicity
        if (!tempSurface)
        {
            SDL_Log("Failed to load surface: %s", SDL_GetError());
            return false;
        }

        // Convert the surface into a texture
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        if (!texture)
        {
            SDL_Log("Failed to create texture: %s", SDL_GetError());
            return false;
        }

        // Store the texture in the map using a unique pointer
        m_Textures[key] = std::unique_ptr<SDL_Texture, SDL_Deleter>(texture);
        return true;
    }

    // Get a texture by its key
    static SDL_Texture* get(const std::string& key)
    {
        auto it = m_Textures.find(key);
        if (it != m_Textures.end())
            return it->second.get();  // Return raw pointer to the texture
        return nullptr;
    }

    // Remove a texture by its key
    static void remove(const std::string& key)
    {
        m_Textures.erase(key);
    }

    // Clear all m_Textures
    static void clear()
    {
        m_Textures.clear();
    }
};
