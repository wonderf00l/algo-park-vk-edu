#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
using namespace std::chrono;

int main() {
    int size = 1000000;
    std::vector<int> vec(size);

    srand((unsigned)time(nullptr));
    for (int i = 0; i != size; ++i) {
        vec[i] = rand() % 100000;
    }

    std::sort(vec.begin(), vec.end(), std::less<int>());

    for (int i = 0; i != size; ++i) {
        (i != size - 1) ? std::cout << vec[i] << ' ' : std::cout << vec[i];
    }
}