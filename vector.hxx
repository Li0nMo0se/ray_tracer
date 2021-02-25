#include <cmath>

namespace space
{
template <unsigned int size, typename T>
Vector<size, T>& Vector<size, T>::operator=(const Vector<size, T>& v)
{
    std::copy(v.vect_.begin(), v.vect_.end(), this->vect_.begin());
    return *this;
}

template <unsigned int size, typename T>
Vector<size, T>::Vector(const Vector<size, T>& v)
{
    *this = v;
}

template <unsigned int size, typename T>
inline Vector<size, T>& Vector<size, T>::operator*=(T rhs)
{
    for (T& elt : this->vect_)
        elt *= rhs;
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
    for (const T elt : *this)
        res += elt * elt;
    return std::sqrt(res);
}

template <unsigned int size, typename T>
inline void Vector<size, T>::normalize()
{
    *this /= this->length();
}

template <unsigned int size, typename T>
inline Vector<size, T> Vector<size, T>::normalized() const
{
    Vector v;
    // Copy coordinates
    v.vect_ = this->vect_;
    v.normalize();
    return v;
}

template <typename T>
Vector3<T> Vector3<T>::cross_product(const Vector3<T>& rhs) const
{
    Vector3<T> res;
    const auto& lhs_vect = this->vect_;
    const auto& rhs_vect = rhs.vect_;
    res.vect_[0] = lhs_vect[1] * rhs_vect[2] - lhs_vect[2] * rhs_vect[1];
    res.vect_[1] = lhs_vect[2] * rhs_vect[0] - lhs_vect[0] * rhs_vect[2];
    res.vect_[2] = lhs_vect[0] * rhs_vect[1] - lhs_vect[1] * rhs_vect[0];
    return res;
}
} // namespace space