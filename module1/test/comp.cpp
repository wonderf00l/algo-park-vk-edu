#include "comp.hpp"

template <typename T>
bool DefaultComparator<T>::operator()(const T& lhs, const T& rhs) const {
    return lhs < rhs;
}