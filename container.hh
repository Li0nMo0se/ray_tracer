#pragma once

#include <ostream>
#include <vector>

namespace space
{
template <unsigned int size, typename T>
class Container
{
  public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    const_iterator begin() const { return vect_.cbegin(); }
    const_iterator end() const { return vect_.cend(); }

    virtual Container& operator=(const Container& v)
    {
        std::copy(v.vect_.begin(), v.vect_.end(), this->vect_.begin());
        return *this;
    }

    Container(const Container& v) { *this = v; }

    template <unsigned int _size, typename _T>
    friend std::ostream& operator<<(std::ostream& os,
                                    const Container<_size, _T>& container);

    virtual ~Container() = default;

  protected:
    Container() = default;

    iterator begin() { return vect_.begin(); }
    iterator end() { return vect_.end(); };

    std::vector<T> vect_ = std::vector<T>(size);
};

template <unsigned int size, typename T>
inline std::ostream& operator<<(std::ostream& os,
                                const Container<size, T>& container)
{
    os << "(";
    auto it = container.begin();
    if (it != container.end())
        os << *it;
    it++;
    for (; it < container.end(); it++)
    {
        os << "," << *it;
    }

    os << ")";
    return os;
}
} // namespace space