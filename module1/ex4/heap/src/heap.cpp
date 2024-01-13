#include "heap.hpp"

// template <typename T, typename Comparator>
// void Heap<T, Comparator>::_resize(size_t new_size) {
//     assert(new_size >= 0 && buf);

//     if (new_size <= heap_capacity) {
//         heap_size = new_size;
//     }
//     else {
//         heap_capacity = std::max(new_size, heap_capacity * 2);
//         T* new_buf = new T[heap_capacity];
//         for (size_t i = 0; i != heap_size; ++i) {
//             new_buf[i] = buf[i];
//         }
//         delete[] buf;
//         buf = new_buf;
//         heap_size = new_size;
//     }
// }

// should we write def comp?