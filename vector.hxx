#include <cmath>
#include <cstring>

namespace space
{
template <unsigned int size, typename T>
inline Vector<size, T>& Vector<size, T>::operator*=(T rhs)
{
    for (unsigned int i = 0; i < size; i++)
        vect_[i] *= rhs;
    return *this;
}

template <unsigned int size, typename T>
inline Vector<size, T> Vector<size, T>::operator*(T rhs) const
{
    Vector<size, T> new_vect = *this;
    new_vect *= rhs;
    return new_vect;
}

template <unsigned int size, typename T>
inline Vector<size, T>& Vector<size, T>::operator/=(T rhs)
{
    rhs = 1 / rhs;
    *this *= rhs;
    return *this;
}

template <unsigned int size, typename T>
inline Vector<size, T> Vector<size, T>::operator/(T rhs) const
{
    Vector<size> new_vect = *this;
    new_vect /= rhs;
    return new_vect;
}

template <unsigned int size, typename T>
inline Vector<size, T>& Vector<size, T>::operator*=(const Vector<size, T>& rhs)
{
    for (unsigned int i = 0; i < size; i++)
        this->vect_[i] *= rhs.vect_[i];
    return *this;
}
template <unsigned int size, typename T>
inline Vector<size, T>
Vector<size, T>::operator*(const Vector<size, T>& rhs) const
{
    Vector<size, T> new_vect = *this;
    new_vect *= rhs;
    return new_vect;
}

template <unsigned int size, typename T>
inline Vector<size, T>& Vector<size, T>::operator/=(const Vector<size, T>& rhs)
{
    for (unsigned int i = 0; i < size; i++)
        this->vect_[i] /= rhs.vect_[i];
    return *this;
}
template <unsigned int size, typename T>
inline Vector<size, T>
Vector<size, T>::operator/(const Vector<size, T>& rhs) const
{
    Vector<size, T> new_vect = *this;
    new_vect /= rhs;
    return new_vect;
}

template <unsigned int size, typename T>
inline Vector<size, T>& Vector<size, T>::operator+=(const Vector<size, T>& rhs)
{
    for (unsigned int i = 0; i < size; i++)
        this->vect_[i] += rhs.vect_[i];
    return *this;
}

template <unsigned int size, typename T>
inline Vector<size, T>
Vector<size, T>::operator+(const Vector<size, T>& rhs) const
{
    Vector<size, T> new_vect = *this;
    new_vect += rhs;
    return new_vect;
}

template <unsigned int size, typename T>
inline Vector<size, T>& Vector<size, T>::operator+=(const T val)
{
    for (unsigned int i = 0; i < size; i++)
        this->vect_[i] += val;
    return *this;
}

template <unsigned int size, typename T>
inline Vector<size, T> Vector<size, T>::operator+(const T val) const
{
    Vector<size, T> new_vect = *this;
    new_vect += val;
    return new_vect;
}

template <unsigned int size, typename T>
inline Vector<size, T>& Vector<size, T>::operator-=(const Vector<size, T>& rhs)
{
    *this += (rhs * (-1));
    return *this;
}
template <unsigned int size, typename T>
inline Vector<size, T>
Vector<size, T>::operator-(const Vector<size, T>& rhs) const
{
    Vector<size> new_vect = *this;
    new_vect -= rhs;
    return new_vect;
}

template <unsigned int size, typename T>
inline T Vector<size, T>::dot(const Vector<size, T>& rhs) const
{
    T res = 0.0f;
    for (unsigned int i = 0; i < size; i++)
        res += this->vect_[i] * rhs.vect_[i];
    return res;
}

template <unsigned int size, typename T>
inline T Vector<size, T>::length() const
{
    T res = 0.0f;
    for (unsigned int i = 0; i < size; i++)
        res += vect_[i] * vect_[i];
    return std::sqrt(res);
}

template <unsigned int size, typename T>
inline void Vector<size, T>::normalize()
{
    *this /= this->length();
}

template <unsigned int size, typename T>
Vector<size, T>& Vector<size, T>::operator=(const Vector<size, T>& vector)
{
    std::memcpy(this->vect_, vector.vect_, sizeof(T) * size);
    return *this;
}

template <unsigned int size, typename T>
Vector<size, T>::Vector(const Vector<size, T>& vector)
{
    std::memcpy(this->vect_, vector.vect_, sizeof(T) * size);
}

template <unsigned int size, typename T>
inline Vector<size, T> Vector<size, T>::normalized() const
{
    Vector v = *this;
    // Copy coordinates
    v.normalize();
    return v;
}

template <unsigned int size, typename T>
inline Vector<size, T> Vector<size, T>::operator-() const
{
    Vector<size, T> new_vect = *this;
    for (unsigned int i = 0; i < size; i++)
        new_vect.vect_[i] = -(this->vect_[i]);
    return new_vect;
}

template <unsigned int size, typename T>
inline constexpr const T&
Vector<size, T>::operator[](const unsigned int pos) const
{
    assert(pos < size && "index out of bound");
    return this->vect_[pos];
}

template <unsigned int size, typename T>
inline constexpr T& Vector<size, T>::operator[](const unsigned int pos)
{
    assert(pos < size && "index out of bound");
    return this->vect_[pos];
}

inline Vector3 cross_product(const Vector3& lhs, const Vector3& rhs)
{
    Vector3 res;
    const auto& lhs_vect = lhs.vect_;
    const auto& rhs_vect = rhs.vect_;
    res.vect_[0] = lhs_vect[1] * rhs_vect[2] - lhs_vect[2] * rhs_vect[1];
    res.vect_[1] = lhs_vect[2] * rhs_vect[0] - lhs_vect[0] * rhs_vect[2];
    res.vect_[2] = lhs_vect[0] * rhs_vect[1] - lhs_vect[1] * rhs_vect[0];
    return res;
}
} // namespace space