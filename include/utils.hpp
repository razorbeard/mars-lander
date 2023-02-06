#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "point.hpp"

#include <SFML/Graphics/Transform.hpp>

#include <ctime>
#include <random>

namespace sf { class Text; class Shape; }

namespace 
{
    std::default_random_engine createRandomEngine()
    {
        auto seed = static_cast<unsigned long>(std::time(nullptr));
        return std::default_random_engine(seed);
    }

    std::default_random_engine randomEngine = createRandomEngine();
}

namespace utils
{
    int uniform(int inclusiveMin, int inclusiveMax);
    double uniform(double inclusiveMin, double exclusiveMax);

    const sf::Transform scaledScreenTransform();
    void centerOrigin(sf::Text& text);
    void centerOrigin(sf::Shape& shape);

    double toRadian(double degree);
    double length(const Point2d& a, const Point2d& b);
    double length(const Point2d& a);
    Point2d lerp(Point2d u, Point2d v, double t);
    bool onSegment(Point2d p, Point2d q, Point2d r);
    int orientation(Point2d p, Point2d q, Point2d r);
    bool doIntersect(Point2d p1, Point2d q1, Point2d p2, Point2d q2);
    Point2d lineLineIntersection(Point2d p1, Point2d p2, Point2d p3, Point2d p4);
}

#endif
