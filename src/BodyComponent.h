#pragma once
#include "Component.h"

class BodyComponent : public Component
{
    double m_X, m_Y;

public:
    BodyComponent(
        GameObject &owner,
        const double x,
        const double y)
    : Component(owner), m_X(x), m_Y(y) {}

    [[nodiscard]] double &x() { return m_X; }
    [[nodiscard]] double &y() { return m_Y; }

    void draw() override {}
    void update () override {}
};
