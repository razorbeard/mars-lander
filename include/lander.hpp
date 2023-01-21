#ifndef LANDER_HPP
#define LANDER_HPP

#include "point.hpp"

#include <vector>
#include <string>

namespace sf { class VertexArray; }

class Lander
{
public:
    Lander(const Point2d& position, const Point2d& velocity, int fuel, int angle, int thrust);
    Lander();
    virtual ~Lander();

    void update(int angle, int thrust, sf::VertexArray& vertices);
    const Polyline trajectoryLine() const;
    bool hasSafelyLanded() const noexcept;
    const Point2d& position() const noexcept;
    const Point2d& velocity() const noexcept;

private:
    Point2d m_position;
    Point2d m_previousPosition;
    Point2d m_velocity;
    Point2d m_acceleration;
    int m_fuel;
    int m_angle;
    int m_thrust;
};

#endif
