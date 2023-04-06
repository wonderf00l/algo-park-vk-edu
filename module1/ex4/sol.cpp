#include <algorithm>
#include <cassert>
#include <iostream>


template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
};

template <typename T>
struct pDescComparator {
    bool operator()(const T& lhs, const T& rhs) const { return *lhs < *rhs; }
};


template <typename T, typename Comparator = DefaultComparator<T>>
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
    Heap(Comparator comparator = Comparator());
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

template <typename T, typename Comparator>
Heap<T, Comparator>::Heap(
    Comparator comparator)
    : heap_size(0), heap_capacity(1), comp(comparator) {
    buf = nullptr;
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
Heap<T, Comparator>::Heap(const Heap<T, Comparator>& other) {
    assert(other.heap_size);

    heap_size = other.heap_size;
    heap_capacity = other.heap_capacity;
    comp = other.comp;

    buf = new T[heap_size];
    _copy(buf, other.buf, heap_size);
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
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::_resize() {
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
        if (!comp(buf[parent], buf[i])) {
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
    size_t new_parent = i;

    if (left < heap_size && comp(buf[left], buf[i])) {
        new_parent = left;
    }
    if (right < heap_size && comp(buf[right], buf[new_parent])) {
        new_parent = right;
    }

    if (new_parent != i) {
        std::swap(buf[i], buf[new_parent]);
        _sift_down(new_parent);
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
    assert(heap_size);
    return buf[0];
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::push(const T& item) {
    if (!buf) {
        buf = new T[heap_capacity];
    }
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





template <typename T>
struct descriptor {
    const T* arr;
    int size;
    int pos;

    descriptor(const T* arr, int size) : arr(arr), size(size), pos(0) {
        assert(arr && size >= 0 && pos < size);
    }
    bool operator<(const descriptor& other) const {
        assert(pos < size && other.pos < other.size);
        return arr[pos] < other.arr[other.pos];
    }
};

void input_sizes(int* sizes, int arr_num) {
    assert(arr_num >= 0);

    for (int i = 0; i != arr_num; ++i) {
        std::cin >> sizes[i];
        assert(sizes[i] >= 0);
    }
}

template <typename T>
void mem_alloc(T** init_arr, const int* sizes, int arr_num) {
    for (int i = 0; i != arr_num; ++i) {
        init_arr[i] = new T[sizes[i]];
    }
}

template <typename T>
void mem_free(T** init_arr, const int* sizes, int arr_num) {
    for (int i = 0; i != arr_num; ++i) {
        delete[] init_arr[i];
    }
    delete[] init_arr;
    delete[] sizes;
}

template <typename T>
int input_arrays(T** init_arr, const int* sizes, int arr_num) {
    int quantity = 0;
    for (int i = 0; i != arr_num; ++i) {
        for (int j = 0; j != sizes[i]; ++j, ++quantity) {
            std::cin >> init_arr[i][j];
        }
    }
    return quantity;
}

template <typename T>
int input_alloc(T** init_arr, int* sizes, int arr_num) {
    assert(arr_num >= 0);

    int quantity = 0;

    for (int i = 0; i != arr_num; ++i) {
        std::cin >> sizes[i];
        assert(sizes[i] >= 0);
        init_arr[i] = new T[sizes[i]];
        for (int j = 0; j != sizes[i]; ++j, ++quantity) {
            std::cin >> init_arr[i][j];
        }
    }
    return quantity;
}

template <typename T, typename Comparator>
void init_heap_with_desc(const T* const* init_arr, const int* sizes, int arr_num,
                         Heap<descriptor<T>*, Comparator>& heap) {
    for (int i = 0; i != arr_num; ++i) {
        heap.push(new descriptor<T>(init_arr[i], sizes[i]));
    }
}

template <typename T, typename Comparator>
void merge_arrays(Heap<descriptor<T>*, Comparator>& heap, int* result) {
    for (int i = 0; !heap.is_empty();) {
        descriptor<T>* desc = heap.pop();
        if (desc->pos < desc->size) {
            result[i++] = desc->arr[desc->pos++];
            desc->pos < desc->size ? heap.push(desc) : delete desc;
        } else {
            delete desc;
        }
    }
}

template <typename T>
void output(const T* arr, int size) {
    for (int i = 0; i != size; ++i) {
        i != size - 1 ? std::cout << arr[i] << ' ' : std::cout << arr[i] << std::endl;
    }
}

int main() {

    int arr_num = 0;

    std::cin >> arr_num;

    int** init_arr = new int*[arr_num];
    int* sizes = new int[arr_num];

    int item_quantity = input_alloc<int>(init_arr, sizes, arr_num);

    Heap<descriptor<int>*, pDescComparator<descriptor<int>*>> heap;
    init_heap_with_desc<int>(init_arr, sizes, arr_num, heap);

    int* result = new int[item_quantity];
    merge_arrays<int>(heap, result);
    output(result, item_quantity);

    mem_free(init_arr, sizes, arr_num);
    delete[] result;
}
