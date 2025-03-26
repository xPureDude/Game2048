#pragma once

#include "SFML/System.hpp" // IWYU pragma: keep

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
            m_velocity.x = std::max(0.f, m_velocity.x);
        }
        else if (m_velocity.x < 0)
        {
            m_velocity.x += m_friction.x;
            m_velocity.x = std::min(0.f, m_velocity.x);
        }

        m_velocity.y += m_friction.y;
        m_velocity.y = std::min(m_maxVelocity.y, m_velocity.y);
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