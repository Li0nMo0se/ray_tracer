#pragma once

#include "container.hh"
#include <ostream>

namespace space
{
template <unsigned int size, typename T = float>
class Vector : public Container<size, T>
{
  protected:
    Vector() = default;

  public:
    inline Vector& operator*=(T rhs);
    inline Vector operator*(T rhs) const;

    inline Vector& operator/=(T rhs);
    inline Vector operator/(T rhs) const;

    inline Vector& operator+=(const Vector& rhs);
    inline Vector operator+(const Vector& rhs) const;

    inline Vector& operator-=(const Vector& rhs);
    inline Vector operator-(const Vector& rhs) const;

    inline T dot(const Vector& rhs) const;

    inline T length() const;

    inline void normalize();
    inline Vector normalized() const;

    virtual ~Vector() = default;
};

template <unsigned int size, typename T = float>
inline std::ostream& operator<<(std::ostream& os, const Vector<size, T>& vect)
{
    os << "V";
    return os << static_cast<Container<size, T>>(vect);
}

template <typename T = float>
class Vector3 : public Vector<3, T>
{
  public:
    Vector3(const T x, const T y, const T z)
    {
        this->vect_[0] = x;
        this->vect_[1] = y;
        this->vect_[2] = z;
    }

    // Default constructor, undefined values
    Vector3()
        : Vector3(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0))
    {
    }

    Vector3 cross_product(const Vector3& rhs) const;

    ~Vector3() = default;

    template <typename _T>
    friend Vector3<_T> operator*(_T lhs, const Vector3<_T>& v);
};

template <typename T = float>
inline Vector3<T> operator*(T lhs, const Vector3<T>& v)
{
    Vector3 res(v);
    res.vect_[0] *= lhs;
    res.vect_[1] *= lhs;
    res.vect_[2] *= lhs;
    return res;
}

using Point3 = Vector3<float>;

template <typename T = float>
class Vector4 : public Vector<4, T>
{
  public:
    Vector4(const T x, const T y, const T z, const T w)
    {
        this->vect_[0] = x;
        this->vect_[1] = y;
        this->vect_[2] = z;
        this->vect_[3] = w;
    }

    // Default constructor, undefined values
    Vector4()
        : Vector4(static_cast<T>(0),
                  static_cast<T>(0),
                  static_cast<T>(0),
                  static_cast<T>(0))
    {
    }

    ~Vector4() = default;
};
} // namespace space
#include "vector.hxx"