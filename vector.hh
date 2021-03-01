#pragma once

#include <array>
#include <cassert>
#include <ostream>
#include <vector>

namespace space
{
template <unsigned int size, typename T = float>
class Vector
{
  protected:
    using iterator = typename std::array<T, size>::iterator;
    iterator begin() { return vect_.begin(); }
    iterator end() { return vect_.end(); };

    std::array<T, size> vect_;

  public:
    Vector(const std::array<T, size>& values)
        : vect_(values)
    {
    }

    Vector() = default;

    Vector& operator=(const Vector&) = default;
    Vector(const Vector&) = default;

    using const_iterator = typename std::array<T, size>::const_iterator;
    const_iterator begin() const { return vect_.cbegin(); }
    const_iterator end() const { return vect_.cend(); }

    inline Vector& operator*=(T rhs);
    inline Vector operator*(T rhs) const;

    inline Vector& operator/=(T rhs);
    inline Vector operator/(T rhs) const;

    inline Vector& operator+=(const Vector& rhs);
    inline Vector operator+(const Vector& rhs) const;

    inline Vector& operator-=(const Vector& rhs);
    inline Vector operator-(const Vector& rhs) const;

    inline const T& operator[](unsigned int i) const;

    inline T dot(const Vector& rhs) const;

    inline T length() const;

    inline void normalize();
    inline Vector normalized() const;

    virtual ~Vector() = default;

    friend Vector<3, float> cross_product(const Vector<3, float>& lhs,
                                          const Vector<3, float>& rhs);
};

template <unsigned int size, typename T = float>
inline std::ostream& operator<<(std::ostream& os, const Vector<size, T>& vect)
{
    os << "(";
    typename Vector<size, T>::const_iterator it = vect.begin();
    if (it != vect.end())
        os << *it;
    it++;
    for (; it < vect.end(); it++)
    {
        os << "," << *it;
    }

    os << ")";
    return os;
}

template <unsigned int size, typename T = float>
inline Vector<size, T> operator*(const float scalar,
                                 const Vector<size, T>& vect)
{
    return vect * scalar;
}

using Vector3 = Vector<3, float>;
using Point3 = Vector3;

inline Vector3 cross_product(const Vector3& lhs, const Vector3& rhs);

} // namespace space
#include "vector.hxx"