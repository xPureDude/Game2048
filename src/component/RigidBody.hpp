#pragma once

#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"

#include <cmath>

namespace comp
{

struct RigidBody
{
    void Update(const sf::Time& elpased)
    {
        if (m_velocity.x > 0)
        {
            m_velocity.x -= m_friction.x;
            if (m_velocity.x < 0)
                m_velocity.x = 0;
        }
        else if (m_velocity.x < 0)
        {
            m_velocity.x += m_friction.x;
            if (m_velocity.x > 0)
                m_velocity.x = 0;
        }

        m_velocity.y += m_friction.y;
        if (m_velocity.y > m_maxVelocity.y)
            m_velocity.y = m_maxVelocity.y;
    }

    sf::Vector2f m_velocity;
    sf::Vector2f m_maxVelocity;
    sf::Vector2f m_friction;
};

struct RigidBody1
{
    RigidBody1()
        : m_mass(0.0f),
          m_gravity(0.0f),
          m_frictionRate(0.0f)
    {
    }

    void Update(const sf::Time& elapsed)
    {
        sf::Vector2f a = m_force / m_mass;

        m_velocity += m_force * (elapsed.asMicroseconds() / 1000000.0f);

        auto friction = _CalculateFriction();
        m_velocity += friction;

        m_force = sf::Vector2f(0, 0);
    }

    void ApplyForce(const sf::Vector2f& force) { m_force += force; }

private:
    sf::Vector2f _CalculateFriction()
    {
        float gravityForce = m_gravity * m_mass;
        float frictionX = gravityForce * m_frictionRate;

        sf::Vector2f antiVelocity(-m_velocity.x, -m_velocity.y);
        float length = std::sqrt(antiVelocity.x * antiVelocity.x + antiVelocity.y * antiVelocity.y);

        if (length > 0)
        {
            antiVelocity.x /= length;
            antiVelocity.y /= length;
        }

        return {frictionX * antiVelocity.x, frictionX * antiVelocity.y};
    }

public:
    sf::Vector2f m_velocity;
    sf::Vector2f m_friction;
    sf::Vector2f m_force;
    float m_mass;
    float m_gravity;
    float m_frictionRate;
};

} // namespace comp