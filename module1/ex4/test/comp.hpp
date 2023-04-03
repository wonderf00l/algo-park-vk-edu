#pragma once

template <typename T>
struct DefaultComparator {
    bool DefaultComparator<T>::operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

// шаблонный компаратор нужен, чтобы задавать алгоритм сравнения при инициализации