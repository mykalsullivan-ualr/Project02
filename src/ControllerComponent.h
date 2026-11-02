#pragma once

#include "Component.h"
#include "BodyComponent.h"
#include "Input.h"
#include "Engine.h"

class ControllerComponent : public Component
{
    double m_Speed;

public:
    explicit ControllerComponent(
        GameObject& parent,
        const double speed = 5.0
    )
        : Component(parent), m_Speed(speed)
    {}

    void update() override
    {
        const auto body = parent().get<BodyComponent>();

        if (!body)
            return;

        // Horizontal movement
        if (Input::isKeyDown(SDLK_a) ||
            Input::isKeyDown(SDLK_LEFT))
        {
            body->x() -= m_Speed;
        }

        if (Input::isKeyDown(SDLK_d) ||
            Input::isKeyDown(SDLK_RIGHT))
        {
            body->x() += m_Speed;
        }

        // Vertical movement
        if (Input::isKeyDown(SDLK_w) ||
            Input::isKeyDown(SDLK_UP))
        {
            body->y() -= m_Speed;
        }

        if (Input::isKeyDown(SDLK_s) ||
            Input::isKeyDown(SDLK_DOWN))
        {
            body->y() += m_Speed;
        }

        // Keep player inside the window
        if (body->x() < 0)
            body->x() = 0;

        if (body->y() < 0)
            body->y() = 0;

        if (body->x() > Engine::width - 64)
            body->x() = Engine::width - 64;
    }

    void draw() override {}
};