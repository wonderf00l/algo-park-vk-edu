#pragma once
// конкретизация шаблона подразумевает, что в ТОМ ЖЕ файле присутствует определение шаблона, ключевое слово
// export делает шаблоны экспортируемыми т.к. определения шаблонов в hpp, заголовочные файлы стандартной
// библиотеки должны быть здесь

// не указывам default args в определении функции в cpp файле, только в hpp
#include <algorithm>
#include <cassert>
#include <iostream>

#include "comp.hpp"

template <typename T, typename Comparator = DefaultComparator<T>>  // should do default comp
class Heap {
   private:
    void _copy(T* dest, const T* src, size_t size);
    void _resize();
    void _sift_up(size_t i);
    void _sift_down(size_t i);
    void _make_heap();

    size_t heap_size;
    size_t heap_capacity;

    Comparator comp;

    T* buf;

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
    void print_heap() const;

    ~Heap();
};

template <typename T, typename Comparator>
void Heap<T, Comparator>::_copy(T* dest, const T* src, size_t size) {
    for (size_t i = 0; i != size; ++i) {
        dest[i] = src[i];
    }
}

template <typename T, typename Comparator> // useful onli with operator[]
Heap<T, Comparator>::Heap(      // maybe should do heap with zero start size to push elements further
    size_t init_size,
    Comparator comparator)
    : heap_size(init_size), heap_capacity(init_size * 2), comp(comparator) {
    assert(heap_size);
    buf = new T[heap_capacity];
}

template <typename T, typename Comparator>
Heap<T, Comparator>::Heap(const T* some_buf, size_t some_buf_size, Comparator comparator)
    : heap_size(some_buf_size), heap_capacity(some_buf_size * 2), comp(comparator) {
    assert(some_buf_size);

    buf = new T[heap_capacity];
    _copy(buf, some_buf, some_buf_size);
    _make_heap();
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::operator=(const Heap<T, Comparator>& other) {
    assert(other.heap_size);

    if (buf) {
        delete[] buf;
    }

    heap_size = other.heap_size;
    heap_capacity = other.heap_capacity;
    comp = other.comp;

    buf = new T[heap_capacity];
    _copy(buf, other.buf, heap_size);
    _make_heap();
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::_resize() {
    // assert(new_size >= 0 && buf);
    if (heap_size == heap_capacity) {
        heap_capacity *= 2;
        T* new_buf = new T[heap_capacity];
        _copy(new_buf, buf, heap_size);
        delete[] buf;
        buf = new_buf;
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::_sift_up(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (comp(buf[parent], buf[i])) {  // parent >= 0
            std::swap(buf[i], buf[parent]);
            i = parent;
        } else {
            return;
        }
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::_sift_down(size_t i) {
    size_t left = i * 2 + 1;
    size_t right = i * 2 + 2;
    size_t largest = i;

    if (left < heap_size && comp(buf[i], buf[left])) {
        largest = left;
    } else if (right < heap_size && comp(buf[i], buf[right])) {
        largest = right;
    }

    if (largest != i) {
        std::swap(buf[i], buf[largest]);
        _sift_down(largest);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::_make_heap() {
    for (int i = heap_size / 2 - 1; i >= 0; --i) {
        _sift_down(i);
    }
}

template <typename T, typename Comparator>
T Heap<T, Comparator>::pop() {
    assert(heap_size);

    T top = buf[0];
    buf[0] = buf[(heap_size--) - 1];
    if (heap_size) {
        _sift_down(0);
    }

    return top;
}

template <typename T, typename Comparator>
const T& Heap<T, Comparator>::peek() const {
    return buf[0];
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::push(const T& item) {
    buf[heap_size++] = item;
    _resize();
    _sift_up(heap_size - 1);
}

template <typename T, typename Comparator>
bool Heap<T, Comparator>::is_empty() const {
    return heap_size == 0;
}

template <typename T, typename Comparator>
size_t Heap<T, Comparator>::size() const {
    return heap_size;
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::print_heap() const {
    for (size_t i = 0; i != heap_size; ++i) {
        (i + 1) != heap_size ? std::cout << buf[i] << ' ' : std::cout << buf[i] << std::endl;
    }
}

template <typename T, typename Comparator>
Heap<T, Comparator>::~Heap() {
    delete[] buf;
}