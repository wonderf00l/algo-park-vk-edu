#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

void input_size(size_t &a_size) { std::cin >> a_size; }

void fill_arr(std::vector<int> &arr) {
    for (int &item : arr) {  // also need ref here
        std::cin >> item;
    }
}

void output(const std::vector<int> &arr) {
    for (size_t i = 0; i != arr.size(); ++i) {
        i != arr.size() - 1 ? std::cout << arr[i] << ' ' : std::cout << arr[i] << std::endl;
    }
}

std::pair<int, int> exp_search(const std::vector<int> &arr, int item) {
    size_t left = 0, right = 1;
    if (item < arr[left]) {
        return std::make_pair(left, left);  //-1 left
    } else if (item > arr[arr.size() - 1]) {
        return std::make_pair(arr.size() - 1, arr.size() - 1);
    }

    while ((right < arr.size()) && (item < arr[left] || item > arr[right])) {
        left = right;
        right *= 2;
    }

    if (right > arr.size() - 1) {
        right = arr.size() - 1;
    }

    return std::make_pair(left, right + 1);
}

size_t lower_bound(const std::vector<int> &arr, size_t left, size_t right, int item) {
    assert(right <= arr.size());
    //    assert(arr.size() && left <= arr.size() && right <= arr.size());

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

size_t find_min_id(const std::vector<int> &A, int arr_item) {
    std::pair<int, int> bounds = exp_search(A, arr_item);
    // std::cout << "arr_item:" << arr_item << std::endl;
    // std::cout << "exp:" << bounds.first << bounds.second << std::endl;
    return lower_bound(A, bounds.first, bounds.second, arr_item);
}

std::vector<int> find_indexes(const std::vector<int> &A, const std::vector<int> &B) {
    std::vector<int> id_arr;
    id_arr.reserve(B.size());

    for (const int arr_item : B) {
        id_arr.push_back(find_min_id(A, arr_item));
    }

    return id_arr;
}

int main() {
    size_t a_size = 0, b_size = 0;
    input_size(a_size);
    input_size(b_size);

    std::vector<int> A(a_size);
    std::vector<int> B(b_size);

    fill_arr(A);
    fill_arr(B);

    output(A);
    output(B);

    std::vector<int> id_arr = find_indexes(A, B);

    output(id_arr);
}