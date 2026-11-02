#pragma once
#include "Component.h"
#include "BodyComponent.h"

class SlideComponent : public Component
{
    double m_Speed;
    bool m_GoingRight {true};
    int m_LeftX;
    int m_RightX;
    
public:
    explicit SlideComponent(GameObject& parent) : Component(parent)
    {
        m_Speed = 5;
        m_LeftX = 0;
        m_RightX = Engine::width;
    }

    SlideComponent(
        GameObject& parent,
        const double speed)
    : Component(parent), m_Speed(speed)
    {
        m_LeftX = 0;
        m_RightX = Engine::width;
    }

    SlideComponent(
        GameObject &parent,
        const double speed,
        const int leftX,
        const int rightX)
    : Component(parent),
    m_Speed(speed), m_LeftX(leftX), m_RightX(rightX)
    {}
    
    void update() override
    {
        const auto body = parent().get<BodyComponent>();
        //body->x()+=100*Engine::deltatime;
        if (body->x() > m_RightX)
        {
            m_GoingRight = false;
            body->x() = m_RightX;
        }
        if (body->x() < m_LeftX )
        {
            m_GoingRight = true;
            body->x() = m_LeftX;
        }


        if (m_GoingRight)
            body->x() += m_Speed;
        else
            body->x() -= m_Speed;
    }

    void draw() override
    {}
};