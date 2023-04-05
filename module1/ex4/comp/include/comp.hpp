#pragma once

template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const;
};

template <typename T>
bool DefaultComparator<T>::operator()(const T& lhs, const T& rhs) const {
    return lhs < rhs;
}
// inline функции должны быть определены в том же файле, где будут подставляться при вызове(т.е. нужно
// вкключить определние в hpp, при Include определние функции уже будет в obj файле и линковщик найдет его)
//inline void f() { int a = 1; }

// extern inline void f1();

// шаблонный компаратор нужен, чтобы задавать алгоритм сравнения при инициализации
