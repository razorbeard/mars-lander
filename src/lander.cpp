#include "lander.hpp"
#include "utils.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <math.h>
#include <algorithm>

namespace 
{
   const double gravity = -3.771; 
}

Lander::Lander(const Point2d& position, const Point2d& velocity, int fuel, int angle, int thrust)
    : m_shape{3}
    , m_position{position}
    , m_previousPosition{position}
    , m_velocity{velocity}
    , m_acceleration{thrust * std::sin(utils::toRadian(-angle)),
                     thrust * std::cos(utils::toRadian(-angle)) + gravity}
    , m_fuel{fuel}
    , m_angle{angle}
    , m_thrust{thrust}
{
    m_shape.setPoint(0, sf::Vector2f(0, 0));
    m_shape.setPoint(1, sf::Vector2f(50, 100));
    m_shape.setPoint(2, sf::Vector2f(100, 0));
    m_shape.setFillColor(sf::Color::Green);
    utils::centerOrigin(m_shape);
    this->setPosition(position.x, position.y);
}

Lander::Lander()
    : m_fuel{0}
    , m_angle{0}
    , m_thrust{0}
{

}

Lander::~Lander()
{

}

void Lander::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Transform transform = states.transform;
    transform *= getTransform();

    target.draw(m_shape, utils::scaledScreenTransform() * transform);
}

void Lander::simulationStep(int angle, int thrust)
{
    m_previousPosition = m_position;

    int clampedAngle = std::clamp(m_angle + angle, m_angle - 15, m_angle + 15);
    int clampedThrust = std::clamp(m_thrust + thrust, m_thrust - 1, m_thrust + 1);

    m_angle = std::clamp(clampedAngle, -90, 90);
    m_thrust = std::clamp(clampedThrust, 0, 4);
    m_fuel -= thrust;

    m_acceleration.x = m_thrust * std::sin(utils::toRadian(-m_angle));
    m_acceleration.y = m_thrust * std::cos(utils::toRadian(-m_angle)) + gravity;
    
    m_velocity.x += m_acceleration.x;
    m_velocity.y += m_acceleration.y;

    m_position.x += m_velocity.x + (0.5 * m_acceleration.x);
    m_position.y += m_velocity.y + (0.5 * m_acceleration.y);
}

const Polyline Lander::trajectoryLine() const
{
    Polyline trajectoryLine;
    trajectoryLine.push_back(m_previousPosition);
    trajectoryLine.push_back(m_position);

    return trajectoryLine;
}

bool Lander::hasSafelyLanded() const noexcept
{
    return (std::abs(m_angle)      <= 15 &&
            std::abs(m_velocity.x) <= 20 &&
            std::abs(m_velocity.y) <= 40);
}

const Point2d& Lander::position() const noexcept
{
    return m_position;
}

const Point2d& Lander::velocity() const noexcept
{
    return m_velocity;
}
