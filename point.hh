#pragma once

#include "container.hh"
#include <ostream>

namespace space
{
  /*
template <unsigned int size, typename T>
class Point : public Container<size, T>
{
  protected:
    Point() = default;
};

template <unsigned int size, typename T>
inline std::ostream& operator<<(std::ostream& os, const Point<size, T>& point)
{
    os << "P";
    return os << static_cast<Container<size, T>>(point);
}

template <typename T>
class Point3 : public Point<3, T>
{
  public:
    Point3(const T x, const T y, const T z)
    {
        this->vect_[0] = x;
        this->vect_[1] = y;
        this->vect_[2] = z;
    }
};

template <typename T>
class Point4 : public Point<4, T>
{
  public:
    Point4(const T x, const T y, const T z, const T w)
    {
        this->vect_[0] = x;
        this->vect_[1] = y;
        this->vect_[2] = z;
        this->vect_[3] = w;
    }
};*/
} // namespace space