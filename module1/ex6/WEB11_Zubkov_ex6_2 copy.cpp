#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
};

template <typename T, typename Comparator = DefaultComparator<T>>
int find_median_three(std::vector<T>& arr, int left, int right, Comparator comp = Comparator()) {
    assert(left < right);
    int middle = (left + right) / 2;
    return comp(arr[left], arr[middle]) ? (comp(arr[middle], arr[right]) ? middle : right)
                                        : (comp(arr[left], arr[right]) ? left : right);
}

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(std::vector<T>& arr, int left, int right, Comparator comp = Comparator()) {
    assert(arr && left < right);
    int pivot_id = find_median_three(arr, left, right);
    assert(pivot_id >= left && pivot_id <= right);

    std::swap(arr[pivot_id], arr[left]);
    pivot_id = left;

    int i = right, j = right;

    while (j > pivot_id) {
        for (; j > pivot_id && comp(arr[j], arr[pivot_id]); --j) {
        };

        if (j > pivot_id) {
            std::swap(arr[i--], arr[j--]);
        }
    }

    assert(i >= pivot_id);

    std::swap(arr[pivot_id], arr[i]);

    return i;
}

template <typename T, typename Comparator = DefaultComparator<T>>
void kth_stat(std::vector<T>& arr, int size, int k, Comparator comp = Comparator()) {
    assert(arr && size >= 0 && k >= 0 && k < size);

    int left = 0, right = size - 1;

    while (left < right) {
        int sorted_pos = partition(arr, left, right, comp);
        if (k == sorted_pos) {
            return;
        } else if (k < sorted_pos) {
            right = sorted_pos - 1;
        } else {
            left = sorted_pos + 1;
        }
    }
}

template <typename T>
void input(T* arr, int size) {
    for (int i = 0; i != size; ++i) {
        std::cin >> arr[i];
    }
}

template <typename T, typename Comparator = DefaultComparator<T>>
void test_kth_stat(T* arr, int size, const T* expected, int k, Comparator comp = Comparator()) {
    kth_stat<T>(arr, size, k, comp);
    assert(arr[k] == expected[k]);
    std::cout << "test with k = " << k << " ok" << std::endl;
}

void run_tests() {
    const int size = 9, size_ = 7, _size = 11, _size_ = 3;

    int arr[] = {4, 1, 10, 9, 7, 12, 8, 2, 15};
    int sorted[] = {1, 2, 4, 7, 8, 9, 10, 12, 15};

    int arr_[] = {0, 10, 3, -1, 1, 20, 2};
    int sorted_[] = {-1, 0, 1, 2, 3, 10, 20};

    int _arr[] = {20, -1, 3, 4, 4, 23, -43, 212, 123, 333, 444};
    int _sorted[] = {-43, -1, 3, 4, 4, 20, 23, 123, 212, 333, 444};

    int _arr_[] = {1, 1, 3};
    int _sorted_[] = {1, 1, 3};

    test_kth_stat<int>(_arr, _size, _sorted, _size * 0.1);
    test_kth_stat<int>(_arr, _size, _sorted, _size * 0.5);
    test_kth_stat<int>(_arr, _size, _sorted, _size * 0.9);

    std::cout << std::endl;

    test_kth_stat<int>(_arr_, _size_, _sorted_, _size_ * 0.1);
    test_kth_stat<int>(_arr_, _size_, _sorted_, _size_ * 0.5);
    test_kth_stat<int>(_arr_, _size_, _sorted_, _size_ * 0.9);

    std::cout << std::endl;

    for (size_t i = 0; i != size; ++i) {
        test_kth_stat<int>(arr, size, sorted, i);
    }

    std::cout << std::endl;

    for (size_t i = 0; i != size_; ++i) {
        test_kth_stat<int>(arr_, size_, sorted_, i);
    }
}

int main() {
    // run_tests();

    int size = 1000000;
    // std::cin >> size;
    // assert(size >= 0);
    srand((unsigned)time(NULL));
    // int* arr = new int[size];
    // input(arr, size);

    std::vector<int> vec(size);

    for (int i = 0; i != size; ++i) {
        vec[i] = rand();
    }

    std::sort(vec.begin(), vec.end(), std::greater<int>());

    for (int i = 0; i != size; ++i) {
        std::cout << vec[i] << ' ';
    }
    std::cout << std::endl;

    kth_stat(vec, size, size * 0.1);
    std::cout << vec[(int)(size * 0.1)] << std::endl;
    kth_stat(vec, size, size * 0.5);
    std::cout << vec[(int)(size * 0.5)] << std::endl;
    kth_stat(vec, size, size * 0.9);
    std::cout << vec[(int)(size * 0.9)] << std::endl;

    // delete[] arr;
}