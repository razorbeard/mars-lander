#ifndef LANDER_HPP
#define LANDER_HPP

#include "point.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include <vector>
#include <string>

namespace sf { class RenderTarget; }

class Lander : public sf::Drawable, public sf::Transformable
{
public:
    Lander(const Point2d& position, const Point2d& velocity, int fuel, int angle, int thrust);
    Lander();
    virtual ~Lander();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void simulationStep(int angle, int thrust);
    const Polyline trajectoryLine() const;
    bool hasSafelyLanded() const noexcept;
    const Point2d& position() const noexcept;
    const Point2d& velocity() const noexcept;

private:
    sf::ConvexShape m_shape;
    Point2d m_position;
    Point2d m_previousPosition;
    Point2d m_velocity;
    Point2d m_acceleration;
    int m_fuel;
    int m_angle;
    int m_thrust;
};

#endif
