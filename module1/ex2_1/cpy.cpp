#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

void input_size(size_t &a_size) { std::cin >> a_size; }

void fill_arr(std::vector<unsigned int> &arr) {
    for (unsigned int &item : arr) {
        std::cin >> item;
    }
}

template <typename T = unsigned int>
void output(const std::vector<T> &arr) {
    for (size_t i = 0; i != arr.size(); ++i) {
        i != arr.size() - 1 ? std::cout << arr[i] << ' ' : std::cout << arr[i] << std::endl;
    }
}

// std::pair<size_t, size_t> exp_search(const std::vector<unsigned int> &arr, unsigned int item) {

//     assert(arr.size());

//     size_t left = 0, right = 1;
//     if (item < arr[left]) {
//         return std::make_pair(left, left);
//     } else if (item > arr[arr.size() - 1]) {
//         return std::make_pair(arr.size(), arr.size());
//     }

//     while ((right < arr.size()) && arr[right] < item) {
//         left = right;
//         right *= 2;
//     }

//     if (right > arr.size() - 1) {
//         right = arr.size() - 1;
//     }
//     //std::cout << left << '\t' << right + 1 << std::endl;
//     return std::make_pair(left, right + 1);
// }

std::pair<size_t, size_t> exp_search(const std::vector<unsigned int> &arr, unsigned int item) {
    assert(arr.size());

    size_t right = 1;
    if (item <= arr[0]) {
        return std::make_pair(0, 0);
    } else if (item > arr[arr.size() - 1]) {
        return std::make_pair(arr.size(), arr.size());
    }

    while ((right < arr.size()) && arr[right] < item) {
        right *= 2;
    }

    // if (right > arr.size() - 1) {
    //     right = arr.size() - 1;
    // }
    // std::cout << left << '\t' << right + 1 << std::endl;
    return std::make_pair(right / 2, std::min(right, arr.size()));
}

size_t lower_bound(const std::vector<unsigned int> &arr, size_t left, size_t right, unsigned int item) {
    assert(right <= arr.size());

    while (left < right) {
        size_t middle = (left + right) / 2;
        if (arr[middle] < item) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    return left;
}

size_t find_min_id(const std::vector<unsigned int> &A, unsigned int arr_item) {
    std::pair<size_t, size_t> bounds = exp_search(A, arr_item);
    return lower_bound(A, bounds.first, bounds.second, arr_item);
}

std::vector<size_t> find_indexes(const std::vector<unsigned int> &A, const std::vector<unsigned int> &B) {
    assert(B.size());

    std::vector<size_t> id_arr;
    id_arr.reserve(B.size());

    for (const unsigned int &arr_item : B) {
        id_arr.push_back(find_min_id(A, arr_item));
    }

    return id_arr;
}

int main() {
    size_t a_size = 0, b_size = 0;
    input_size(a_size);
    input_size(b_size);

    std::vector<unsigned int> A(a_size);
    std::vector<unsigned int> B(b_size);

    fill_arr(A);
    fill_arr(B);

    std::vector<size_t> id_arr = find_indexes(A, B);

    output<size_t>(id_arr);
}