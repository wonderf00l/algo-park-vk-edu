#include <cassert>
#include <iostream>
#include <vector>

void input(std::vector<int>& arr) {
    for (int& item : arr) {
        std::cin >> item;
    }
}

size_t lower_bound(const std::vector<int>& arr, size_t left, size_t right, int item) {
    assert(arr.size() && left <= arr.size() && right <= arr.size());

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

int main() {
    size_t size = 0;
    std::cin >> size;

    std::vector<int> arr(size);
    input(arr);

    int k = 0;

    std::cin >> k;

    std::cout << lower_bound(arr, 0, arr.size(), k) << std::endl;
}