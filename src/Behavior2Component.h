#pragma once

#include "Component.h"
#include "BodyComponent.h"
#include "GameObject.h"
#include <cmath>

class Behavior2Component : public Component
{
    GameObject& m_Player;
    double m_Speed;
    double m_StopDistance;

public:
    Behavior2Component(
        GameObject& parent,
        GameObject& player,
        const double speed = 2.0,
        const double stopDistance = 100.0
    )
        : Component(parent),
          m_Player(player),
          m_Speed(speed),
          m_StopDistance(stopDistance)
    {}

    void update() override
    {
        const auto body = parent().get<BodyComponent>();
        const auto playerBody = m_Player.get<BodyComponent>();

        if (!body || !playerBody)
            return;

        double dx = playerBody->x() - body->x();
        double dy = playerBody->y() - body->y();

        double distance = std::sqrt(dx * dx + dy * dy);

        // Stop when close enough
        if (distance <= m_StopDistance || distance == 0.0)
            return;

        // Normalize direction
        dx /= distance;
        dy /= distance;

        // Move toward player
        body->x() += dx * m_Speed;
        body->y() += dy * m_Speed;
    }

    void draw() override {}
};
