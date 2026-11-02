#pragma once

#include "Component.h"
#include "GameObject.h"
#include "BodyComponent.h"
#include <cmath>

class Behavior1Component : public Component
{
    GameObject &m_Player;

    double m_Speed;
    double m_DetectionRange;

public:
    Behavior1Component(
        GameObject &parent,
        GameObject &player,
        const double speed = 3.0,
        const double detectionRange = 200.0
    )
        : Component(parent),
          m_Player(player),
          m_Speed(speed),
          m_DetectionRange(detectionRange)
    {}

    void update() override
    {
        const auto body = parent().get<BodyComponent>();
        const auto playerBody = m_Player.get<BodyComponent>();

        if (!body || !playerBody)
            return;

        double dx = body->x() - playerBody->x();
        double dy = body->y() - playerBody->y();

        // Only flee when the player is nearby
        if (const double distance = std::sqrt(dx * dx + dy * dy); distance < m_DetectionRange && distance > 0.0)
        {
            // Normalize direction
            dx /= distance;
            dy /= distance;

            // Move away from player
            body->x() += dx * m_Speed;
            body->y() += dy * m_Speed;
        }
    }

    void draw() override {}
};
