#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>

template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
int find_median_three(T* arr, int left,int right, Comparator comp = Comparator()) {
    assert(left < right);
    int first = left, last = right, median = (last - first) / 2;
    return comp(arr[first], arr[median]) ? (comp(arr[median], arr[last]) ? median : last) : (comp(arr[first], arr[last]) ? first : last);
}

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(T* arr, int left, int right, Comparator comp = Comparator()) {
    
    assert(left < right);
    int pivot_id = find_median_three(arr, left, right);

    std::swap(arr[pivot_id], arr[left]);
    pivot_id = left;

    ++left;

    while (left < right) {
        for (;comp(arr[left], arr[pivot_id]); ++left) {};
        for (;!comp(arr[right], arr[pivot_id]); --right) {};

        if (left < right) {
            std::swap(arr[left++], arr[right--]);
        }
    }

    for (;comp(arr[pivot_id], arr[right]); --right) {}; // should be there

    std::swap(arr[pivot_id], arr[right]);

    return right;
}

template <typename T, typename Comparator = DefaultComparator<T>>
void kth_stat(T* arr, int size, int k, Comparator comp = Comparator()) {

    assert(size >= 0 && k >= 0 && k < size);

    int left = 0, right = size - 1;

    while (left < right) {
        int sorted_pos = partition(arr, left, right, comp); // копирование left/right, в namespace kth_stat остаются неизменны после работы partition
        if (k == sorted_pos) {
            return;
        }
        if (k < sorted_pos) {
            right = sorted_pos - 1;
            //partition(arr, left, right, comp);
        }
        if (k > sorted_pos) {
            left = sorted_pos + 1;
            //partition(arr, left, right, comp);
        }
        std::cout << "left: " << left << " " << "right: " << right << std::endl;
    }
}

void test_kth_stat() {

    std::vector<int> vec = {4, 1, 10, 9, 7, 12, 8, 2, 15};
    std::vector<int> v = {0, 10, 3, -1 ,1, 20 ,2};
    std::vector<int> v1 = {3, 2, 1, 0, 0 ,0, 22};

    std::vector<int> expected(v);
    std::sort(expected.begin(), expected.end());

    //std::cout << find_median_three(&v[0], 0, v.size() - 1) << std::endl;

    //std::cout << partition(&vec[0], 0, vec.size() - 1) << ' ' << vec[partition(&vec[0], 0, vec.size() - 1)] << vec[partition(&vec[0], 0, vec.size() - 1)] << std::endl;

    kth_stat<int>(&v[0], v.size(), 2);

    assert(v[2] == expected[2]);

    //should right lambda


}

int main () {
    test_kth_stat();
}