#include <iostream>

#include "comp.hpp"
#include "heap.hpp"

// полезная команда для сборки: g++ -c -Wextra -Wall -Werror -std=c++17 -fsanitize=address *.cpp && g++
// -Wextra -Wall -Werror -fsanitize=address *.o -o executable g++ -o executable -Wextra -Wall -Werror
// -std=c++17 -fsanitize=address *.cpp -I./
int main() {
    Heap<int, DefaultComparator<int>> h = Heap<int, DefaultComparator<int>>();

    f();

    // Heap<int, DefaultComparator<int>> he = Heap<int, DefaultComparator<int>>(5);

    // int arr[] = {2, -1, 3, 4, 44, 184, -24, 0, 0};
    // int arr_[] = {0, 1, 2, 3};
    // int arr__[] = {1};
    // int arr___[] = {1, 2};
    // int _arr[0];

    // Heap<int, DefaultComparator<int>> h1 = Heap<int, DefaultComparator<int>>(arr, 9);
    // Heap<int, DefaultComparator<int>> h2 = Heap<int, DefaultComparator<int>>(arr_, 4);
    // Heap<int, DefaultComparator<int>> h3 = Heap<int, DefaultComparator<int>>(arr__, 1);
    // Heap<int, DefaultComparator<int>> h4 = Heap<int, DefaultComparator<int>>(arr___, 2);
    // Heap<int, DefaultComparator<int>> h1 = Heap<int, DefaultComparator<int>>(arr, 0);
}