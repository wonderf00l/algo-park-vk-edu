#pragma once

#include <iostream>
#include "comp.hpp"

template <typename T, typename Comparator = DefaultComparator<T>>  // should do default comp
class Heap {
   private:
    void _copy(T* dest, const T* src, size_t size);
    void _resize();
    void _sift_up(size_t i);
    void _sift_down(size_t i);

    Comparator comp;

    T* buf;
    size_t heap_size;
    size_t heap_capacity;

   public:
    Heap(size_t init_size = 1, Comparator comparator = Comparator());
    Heap(const T* some_buf, size_t some_buf_size, Comparator comparator = Comparator());
    Heap(const Heap<T, Comparator>& other);

    void operator=(const Heap<T, Comparator>& other);

    const T& peek() const;
    T pop();
    void push(const T& item);

    bool is_empty() const;
    size_t size() const;

    ~Heap();
};