#include <algorithm>
#include <cassert>
#include <iostream>

template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
};

template <typename T, typename Comparator = DefaultComparator<T>>
int find_median_three(T* arr, int left, int right, Comparator comp = Comparator()) {
    assert(left < right);
    int middle = (left + right) / 2;  // нужно учитывать не абсолютную, а относительную позицию
    return comp(arr[left], arr[middle]) ? (comp(arr[middle], arr[right]) ? middle : right)
                                        : (comp(arr[left], arr[right]) ? left : right);
}

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(T* arr, int left, int right, Comparator comp = Comparator()) {
    assert(arr && left < right);
    int pivot_id = find_median_three(arr, left, right);
    assert(pivot_id >= left && pivot_id <= right);
    // std::cout << "pivot_id: " << pivot_id << std::endl;

    std::swap(arr[pivot_id], arr[left]);
    pivot_id = left;

    int i = right, j = right;

    // std::cout << "left: " << left << "right: " << right << std::endl;

    while (j > pivot_id) {
        for (; j > pivot_id && comp(arr[j], arr[pivot_id]); --j) {
        };

        if (j > pivot_id) {  // проверка должна быть: иначе j после цикла for может стать == pivot_id --> i
                             // уменьшиться в резульате свапа и тем самым сьъедет влево на 1 позицию от
                             // искомой, вне while произойдет некорректный swap
            std::swap(arr[i--], arr[j--]);
        }
    }

    assert(i >= pivot_id);

    std::swap(arr[pivot_id], arr[i]);

    return i;
}

template <typename T, typename Comparator = DefaultComparator<T>>
void kth_stat(T* arr, int size, int k, Comparator comp = Comparator()) {
    assert(arr && size >= 0 && k >= 0 && k < size);

    int left = 0, right = size - 1;

    while (left < right) {
        int sorted_pos = partition(
            arr, left, right,
            comp);  // копирование left/right, в namespace kth_stat остаются неизменны после работы partition
        if (k == sorted_pos) {
            return;
        } else if (k < sorted_pos) {
            right = sorted_pos - 1;
        } else {
            left = sorted_pos + 1;
        }
        // std::cout << "left: " << left << " " << "right: " << right << std::endl;
    }
}

template <typename T, typename Comparator = DefaultComparator<T>>
void test_kth_stat(T* arr, int size, const T* expected, int k, Comparator comp = Comparator()) {
    kth_stat<T>(arr, size, k, comp);
    assert(arr[k] == expected[k]);
    std::cout << "test with k = " << k << " ok" << std::endl;

    // can be lambda instead
}

int main() {
    const int size = 9, size_ = 7, _size = 11;

    int arr[] = {4, 1, 10, 9, 7, 12, 8, 2, 15};
    int sorted[] = {1, 2, 4, 7, 8, 9, 10, 12, 15};

    int arr_[] = {0, 10, 3, -1, 1, 20, 2};
    int sorted_[] = {-1, 0, 1, 2, 3, 10, 20};

    int _arr[] = {20, -1, 3, 4, 4, 23, -43, 212, 123, 333, 444};
    int _sorted[] = {-43, -1, 3, 4, 4, 20, 23, 123, 212, 333, 444};

    test_kth_stat<int>(_arr, _size, _sorted, _size * 0.1);
    test_kth_stat<int>(_arr, _size, _sorted, _size * 0.5);
    test_kth_stat<int>(_arr, _size, _sorted, _size * 0.9);

    std::cout << std::endl;

    for (size_t i = 0; i != size; ++i) {
        test_kth_stat<int>(arr, size, sorted, i);
    }

    std::cout << std::endl;

    for (size_t i = 0; i != size_; ++i) {
        test_kth_stat<int>(arr_, size_, sorted_, i);
    }
}