#pragma once
#include "Component.h"
#include "BodyComponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include "Textures.h"

class SpriteComponent : public Component
{
    SDL_Texture *texture {};

public:
    SpriteComponent(
        GameObject& parent,
        const std::string &textureKey)
    : Component(parent)
    {
        texture = Textures::get(textureKey);
        if (!texture)
            SDL_Log("Failed to load texture for key: %s", textureKey.c_str());
    }
    void update() override {}

    void draw() override
    {
        if (const auto body = parent().get<BodyComponent>())
        {
            const SDL_Rect dst = { static_cast<int>(body->x()), static_cast<int>(body->y()), 64, 64 };
            SDL_RenderCopy(Engine::getRenderer(), texture, nullptr, &dst);
        }
    }
};