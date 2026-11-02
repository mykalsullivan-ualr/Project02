#pragma once

class GameObject;

class Component
{
    GameObject &m_Parent;

protected:
    [[nodiscard]] GameObject &parent() const { return m_Parent; }

public:
    explicit Component(GameObject &parent) : m_Parent(parent) {}
    virtual ~Component() = default;

    virtual void update() = 0;
    virtual void draw() = 0;
};
