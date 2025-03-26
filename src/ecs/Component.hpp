#pragma once

#include "SFML/System.hpp" // IWYU pragma: keep
#include "tinyxml/tinyxml2.h"

class PositionComponent
{
public:
    PositionComponent()
        : m_elevation(0)
    {
    }
    virtual ~PositionComponent() {}

    void Reset();
    bool LoadFromXml(tinyxml2::XMLElement* e);

private:
    uint32_t m_elevation;
    sf::Vector2f m_position;
    sf::Vector2f m_positionOld;
};

class MoveComponent
{
public:
    MoveComponent() {}
    virtual ~MoveComponent() {}

    void Reset();
    bool LoadFromXml(tinyxml2::XMLElement* e);

private:
    sf::Vector2f m_velocity;
    sf::Vector2f m_maxVelocity;
};