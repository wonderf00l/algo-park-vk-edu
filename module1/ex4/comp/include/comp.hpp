#pragma once

template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const;
};

// шаблонный компаратор нужен, чтобы задавать алгоритм сравнения при инициализации
#include "comp.cpp"