#ifndef POINT_HPP
#define POINT_HPP

#include <vector>

template <typename T>
struct Point2
{
    Point2();
    Point2(T X, T Y);

    T x;
    T y;
};

template <typename T>
Point2<T>::Point2() :
    x(0.0),
    y(0.0)
{

}

template <typename T>
Point2<T>::Point2(T X, T Y) :
    x(X),
    y(Y)
{

}

template <typename T>
Point2<T>& operator +=(Point2<T>& lhs, const Point2<T>& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
}

using Point2d = Point2<double>;
using Polyline = std::vector<Point2d>;

#endif
