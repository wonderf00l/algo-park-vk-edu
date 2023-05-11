#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std::chrono;

template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
};

// template <typename T, typename Comparator = DefaultComparator<T>>
// int find_median_three(std::vector<T>& arr, int left, int right, Comparator comp = Comparator()) {
//     assert(left < right);
//     int middle = (left + right) / 2;
//     return comp(arr[left], arr[middle]) ? (comp(arr[middle], arr[right]) ? middle : right)
//                                         : (comp(arr[left], arr[right]) ? left : right);
// }

template <typename T, typename Comparator = DefaultComparator<T>>
size_t find_median_three(std::vector<T>& data, size_t l, size_t r) {
    size_t half = (l + r - 1) / 2;  // середина списка
    // выбор среднего из трех элементов
    if (data[l] < data[half]) {
        if (data[r] < data[l]) {
            return l;
        } else if (data[r] < data[half]) {
            return r;
        }
        return half;
    } else {
        if (data[r] < data[half]) {
            return half;
        } else if (data[r] < data[l]) {
            return r;
        }
        return l;
    }
}

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(std::vector<T>& arr, int left, int right, Comparator comp = Comparator()) {
    // assert(arr && left < right);
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
    // assert(arr && size >= 0 && k >= 0 && k < size);

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
        vec[i] = rand() % 10000;
    }

    std::sort(vec.begin(), vec.end(), std::greater<int>());

    // for (int i = 0; i != size; ++i) {
    //     std::cout << vec[i] << ' ';
    // }
    std::cout << std::endl;

    auto start = high_resolution_clock::now();

    kth_stat(vec, size, size * 0.1);
    std::cout << vec[(int)(size * 0.1)] << std::endl;
    kth_stat(vec, size, size * 0.5);
    std::cout << vec[(int)(size * 0.5)] << std::endl;
    kth_stat(vec, size, size * 0.9);
    std::cout << vec[(int)(size * 0.9)] << std::endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

    // delete[] arr;
}