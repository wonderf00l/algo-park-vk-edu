#include <iostream>

#include "comp.hpp"
#include "heap.hpp"

// полезная команда для сборки: g++ -c -Wextra -Wall -Werror -std=c++17 -fsanitize=address *.cpp && g++
// -Wextra -Wall -Werror -fsanitize=address *.o -o executable g++ -o executable -Wextra -Wall -Werror
// -std=c++17 -fsanitize=address *.cpp -I./

//для сборки a.out сразу
//g++ -Icomp/include/ -Iheap/include/ src/main.cpp comp/src/comp.cpp heap/src/heap.cpp 

//конкретизация имени executable
//g++ -Icomp/include/ -Iheap/include/ src/main.cpp comp/src/comp.cpp heap/src/heap.cpp -o executable

//добавление флажков -c -s и тп приведет к промежуточным этапам
//-g - для сохранения и отображения отоладочной информации

//если несколько headers, то несколько флагов -I, причем указываем с точностью до директории с hpp файлом
//аналогично с указанием source(cpp) файлов

// g++ -Wextra -Wall -Werror -std=c++17 -fsanitize=address -I comp/include/ -I heap/include/ src/main.cpp comp/src/*.cpp heap/src/*.cpp -o executable
int main() {
    Heap<int, DefaultComparator<int>> h;


    Heap<int, DefaultComparator<int>> he = Heap<int>(5);

    int arr[] = {2, -1, 3, 4, 44, 184, -24, 0, 0};
    int arr_[] = {0, 1, 2, 3};
    int arr__[] = {1};
    int arr___[] = {1, 2};
    int* a = new int(5);
    *a = 2;

    Heap<int, DefaultComparator<int>> h1 = Heap<int>(arr, 9);
    Heap<int, DefaultComparator<int>> h2 = Heap<int, DefaultComparator<int>>(arr_, 4);
    Heap<int, DefaultComparator<int>> h3 = Heap<int, DefaultComparator<int>>(arr__, 1);
    Heap<int, DefaultComparator<int>> h4 = Heap<int, DefaultComparator<int>>(arr___, 2);

    he.print_heap();
    h1.print_heap();
    h2.print_heap();
    h3.print_heap();
    h4.print_heap();
}