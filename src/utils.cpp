#include "utils.hpp"

#include <SFML/Graphics/Text.hpp>

namespace utils
{
    int uniform(int inclusiveMin, int inclusiveMax)
    {
        std::uniform_int_distribution<int> distribution(inclusiveMin, inclusiveMax);
        return distribution(randomEngine);
    }

    double uniform(double inclusiveMin, double exclusiveMax)
    {
        std::uniform_real_distribution<double> distribution(inclusiveMin, exclusiveMax);
        return distribution(randomEngine);
    }

    const sf::Transform scaledScreenTransform()
    {
        sf::Transformable result;
        const float height = 3000.0f;
        const float scaledHeight = 428.0f;
        result.setOrigin({0.0f, 7.0f * scaledHeight});
        result.setScale({scaledHeight/height, -scaledHeight/height});

        return result.getTransform();
    }

    void centerOrigin(sf::Text& text)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(std::floor(bounds.left + bounds.width / 2.0f),
                                    std::floor(bounds.top + bounds.height / 2.0f)));
    }

    double toRadian(double degree)
    {
        return 3.14159265358979323846f / 180.0f * degree;
    }

    double length(const Point2d& a, const Point2d& b)
    {
        return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    }
    
    double length(const Point2d& a)
    {
        return std::sqrt(a.x * a.x + a.y * a.y);
    }

    bool onSegment(Point2d p, Point2d q, Point2d r)
    {
        if (q.x <= std::max(p.x, r.x) &&
            q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) &&
            q.y >= std::min(p.y, r.y))
            return true;

        return false;
    }

    int orientation(Point2d p, Point2d q, Point2d r)
    {
        const int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

        if (val == 0)
            return 0; // collinear

        return (val > 0) ? 1 : 2; // clock or counterclock wise
    }

    bool doIntersect(Point2d p1, Point2d q1, Point2d p2, Point2d q2)
    {
        // Find the four orientations needed for general and
        // special cases
        const int o1 = orientation(p1, q1, p2);
        const int o2 = orientation(p1, q1, q2);
        const int o3 = orientation(p2, q2, p1);
        const int o4 = orientation(p2, q2, q1);

        // General case
        if (o1 != o2 && o3 != o4)
            return true;

        // Special Cases
        // p1, q1 and p2 are collinear and p2 lies on segment p1q1
        if (o1 == 0 && onSegment(p1, p2, q1))
            return true;

        // p1, q1 and q2 are collinear and q2 lies on segment p1q1
        if (o2 == 0 && onSegment(p1, q2, q1))
            return true;

        // p2, q2 and p1 are collinear and p1 lies on segment p2q2
        if (o3 == 0 && onSegment(p2, p1, q2))
            return true;

        // p2, q2 and q1 are collinear and q1 lies on segment p2q2
        if (o4 == 0 && onSegment(p2, q1, q2))
            return true;

        return false;  // Doesn't fall in any of the above cases
    }

    Point2d lineLineIntersection(Point2d p1, Point2d p2, Point2d p3, Point2d p4)
    {
        const double nomX = (p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x);
        const double denX = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

        const double nomY = (p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x);
        const double denY = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);

        return {nomX / denX, nomY / denY};
    }
}
