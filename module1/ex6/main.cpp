#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
};

template <typename T>
struct IsGreaterComp {
    bool operator()(const T& lhs, const T& rhs) const { return lhs > rhs; }
};

template <typename T, typename Comparator = DefaultComparator<T>>
int find_median_three(T* arr, int left, int right, Comparator comp = Comparator()) {
    assert(left < right);
    int first = left, last = right, median = (last - first) / 2;
    return comp(arr[first], arr[median]) ? (comp(arr[median], arr[last]) ? median : last)
                                         : (comp(arr[first], arr[last]) ? first : last);
}

//max min funcs
template <typename T, typename Comparator = DefaultComparator<T>>
int marginal_id(T* arr, int size, Comparator comp = Comparator()) {
    int marginal_id = 0;
    for (int i = 1; i != size; ++i) {
        if (comp(arr[i], arr[marginal_id])) {
            marginal_id = i;
        }
    }
    return marginal_id;
}

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(T* arr, int left, int right, Comparator comp = Comparator()) {
    assert(left < right);
    int pivot_id = find_median_three(arr, left, right);
    // std::cout << "pivot_id: " << pivot_id << std::endl;

    std::swap(arr[pivot_id], arr[left]);
    pivot_id = left;

    int i = left + 1;
    int j = right;
    
    std::cout << "left: " << left << "right: " << right << std::endl;

    while (i < j) {
        for (; i <= right && comp(arr[i], arr[pivot_id]); ++i) {
        };
        for (; j > 0 && !comp(arr[j], arr[pivot_id]); --j) {
        };

        if (i < j) {
            std::swap(arr[i++], arr[j--]);
        }
    }

    for (; j > 0 && comp(arr[pivot_id], arr[j]); --j) {
    };  // should be there

    std::swap(arr[pivot_id], arr[j]);

    return j;
}

template <typename T, typename Comparator = DefaultComparator<T>>
void kth_stat(T* arr, int size, int k, Comparator comp = Comparator()) {
    assert(size >= 0 && k >= 0 && k < size);

    if (!k) {
        std::swap(arr[0], arr[marginal_id(arr, size)]);
        return;
    }
    else if (k == size - 1) {
        std::swap(arr[size - 1], arr[marginal_id(arr, size, IsGreaterComp<T>())]);
        return;
    }

    int left = 0, right = size - 1;

    while (left < right) {
        int sorted_pos = partition(
            arr, left, right,
            comp);  // копирование left/right, в namespace kth_stat остаются неизменны после работы partition
        if (k == sorted_pos) {
            return;
        }
        else if (k < sorted_pos) {
            right = sorted_pos - 1;
            // partition(arr, left, right, comp);
        }
        else {
            left = sorted_pos + 1;
            // partition(arr, left, right, comp);
        }
        // std::cout << "left: " << left << " " << "right: " << right << std::endl;
    }
}

template <typename T, typename Comparator = DefaultComparator<T>>
void test_kth_stat(T* arr, int size, const T* expected, int k,
                   Comparator comp = Comparator()) {
    kth_stat<T>(arr, size, k, comp);
    assert(arr[k] == expected[k]);
    std::cout << "test with k = " << k << " ok" << std::endl;

    // can be lambda instead
}


int main() {
    std::vector<int> vec = {4, 1, 10, 9, 7, 12, 8, 2, 15};
    std::vector<int> v = {0, 10, 3, -1, 1, 20, 2};
    std::vector<int> v1 = {2, 1};
    std::vector<int> expected(vec);
    std::sort(expected.begin(), expected.end());

    const int size = 9, size_ = 7;

    int arr[] = {4, 1, 10, 9, 7, 12, 8, 2, 15};
    int sorted[] = {1, 2, 4, 7, 8 ,9 , 10, 12 ,15};

    int arr_[] = {0, 10, 3, -1, 1, 20, 2};
    int sorted_[] = {-1, 0, 1, 2, 3, 10, 20};

    // int id = partition<int>(&v1[0], 0, v1.size() - 1);
    // std::cout << id << ' ' <<  v1[id] << std::endl;

    for (size_t i = 0; i != size; ++i) {
        test_kth_stat<int>(arr,size, sorted, i);
    }

    std::cout << std::endl;

    for (size_t i = 0; i != size_; ++i) {
        test_kth_stat<int>(arr_,size_, sorted_, i);
    }
    // test_kth_stat<int>(vec, expected, 0);
    // test_kth_stat<int>(vec, expected, 3);
    // test_kth_stat<int>(vec, expected, 1); // 0 1 2
    // test_kth_stat<int>(vec, expected, 3);
    // test_kth_stat<int>(vec, expected, 4);
    // test_kth_stat<int>(vec, expected, 5);
    // test_kth_stat<int>(vec, expected, 6);
    // test_kth_stat<int>(vec, expected, 7);
    // test_kth_stat<int>(vec, expected, 8);

    // kth_stat(arr, 9, 0);
    // assert(arr[0] == sorted[0]);
    // kth_stat(arr, 9, 1);
    // assert(arr[1] == sorted[1]);
    // kth_stat(arr, 9, 8);
    // assert(arr[8] == sorted[8]);
    // kth_stat(arr, 9, 5);
    // assert(arr[5] == sorted[5]);
    // kth_stat(arr, 9, 6);
    // assert(arr[6] == sorted[6]);
    // kth_stat(arr, 9, 7);
    // assert(arr[7] == sorted[7]);

}