#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "comp.hpp"
#include "heap.hpp"

// полезная команда для сборки: g++ -c -Wextra -Wall -Werror -std=c++17 -fsanitize=address *.cpp && g++
// -Wextra -Wall -Werror -fsanitize=address *.o -o executable g++ -o executable -Wextra -Wall -Werror
// -std=c++17 -fsanitize=address *.cpp -I./

//для сборки a.out сразу
// g++ -Icomp/include/ -Iheap/include/ src/main.cpp comp/src/comp.cpp heap/src/heap.cpp

//конкретизация имени executable
// g++ -Icomp/include/ -Iheap/include/ src/main.cpp comp/src/comp.cpp heap/src/heap.cpp -o executable

//добавление флажков -c -s и тп приведет к промежуточным этапам
//-g - для сохранения и отображения отоладочной информации

//если несколько headers, то несколько флагов -I, причем указываем с точностью до директории с hpp файлом
//аналогично с указанием source(cpp) файлов

// g++ -Wextra -Wall -Werror -std=c++17 -fsanitize=address -I comp/include/ -I heap/include/ src/main.cpp
// comp/src/*.cpp heap/src/*.cpp -o executable

// to see pointer as an array: *(int(*)[10])some_pointer

template <typename T>
struct descriptor {
    const T* arr;
    int size;
    int pos;

    descriptor(const T* arr, int size) : arr(arr), size(size), pos(0) {
        assert(arr && size >= 0 && pos < size);
    }
    bool operator<(const descriptor& other) const {
        assert(pos < size && other.pos < other.size);
        // std::cout << "pos:\t" << pos << ' ' << "size:\t" << size << ' ' << "other pos:\t" << other.pos << '
        // '
        //           << "other size:\t" << other.size << std::endl;
        return arr[pos] < other.arr[other.pos];
    }
};

void input_sizes(int* sizes, int arr_num) {
    assert(arr_num >= 0);

    for (int i = 0; i != arr_num; ++i) {
        std::cin >> sizes[i];  // OK
        assert(sizes[i] >= 0);
    }
}

template <typename T>
void mem_alloc(T** init_arr, const int* sizes, int arr_num) {
    for (int i = 0; i != arr_num; ++i) {
        init_arr[i] = new T[sizes[i]];
    }
}

template <typename T>
void mem_free(T** init_arr, const int* sizes, int arr_num) {
    for (int i = 0; i != arr_num; ++i) {
        delete[] init_arr[i];
    }
    delete[] init_arr;
    delete[] sizes;
}

template <typename T>
int input_arrays(T** init_arr, const int* sizes, int arr_num) {
    int quantity = 0;
    for (int i = 0; i != arr_num; ++i) {
        for (int j = 0; j != sizes[i]; ++j, ++quantity) {
            std::cin >> init_arr[i][j];
        }
    }
    return quantity;
}

template <typename T>
int input_alloc(T** init_arr, int* sizes, int arr_num) {
    assert(arr_num >= 0);

    int quantity = 0;

    for (int i = 0; i != arr_num; ++i) {
        std::cin >> sizes[i];
        assert(sizes[i] >= 0);
        init_arr[i] = new T[sizes[i]];
        for (int j = 0; j != sizes[i]; ++j, ++quantity) {
            std::cin >> init_arr[i][j];
        }
    }
    return quantity;
}

template <typename T, typename Comparator>
void init_heap_with_desc(const T* const* init_arr, const int* sizes, int arr_num,
                         Heap<descriptor<T>*, Comparator>& heap) {
    for (int i = 0; i != arr_num; ++i) {
        heap.push(new descriptor<T>(init_arr[i], sizes[i]));
    }
}

template <typename T, typename Comparator>
void merge_arrays(Heap<descriptor<T>*, Comparator>& heap, int* result) {
    for (int i = 0; !heap.is_empty();) {
        descriptor<T>* desc = heap.pop();
        if (desc->pos < desc->size) {
            result[i++] = desc->arr[desc->pos++];
            desc->pos < desc->size ? heap.push(desc) : delete desc;
        } else {
            delete desc;
        }
    }
}

template <typename T>
void output(const T* arr, int size) {
    for (int i = 0; i != size; ++i) {
        i != size - 1 ? std::cout << arr[i] << ' ' : std::cout << arr[i] << std::endl;
    }
}

int main() {
    int arr[] = {2, -1, 3, 4, 44, 184, -24, 0, 0};
    int arr_[] = {0, 1, 2, 3};  // 0 3 2 1 -- 3 0 2 1 -- 3 1 2 0

    // std::vector<int> vec = {2, -1, 3, 4, 44, 184, -24, 0, 0};
    // std::make_heap(vec.begin(), vec.end(), std::greater());
    // for (int i : vec) {
    //     std::cout << i << '_';
    // }
    // std::cout << std::endl;

    Heap<int, DefaultComparator<int>> h1 = Heap<int>(arr, 9);
    Heap<int, DefaultComparator<int>> h2 = Heap<int, DefaultComparator<int>>(arr_, 4);

    // h1.print_heap();

    int arr_num = 0;

    std::cin >> arr_num;

    int** init_arr = new int*[arr_num];
    int* sizes = new int[arr_num];

    // input_sizes(sizes, arr_num);
    // mem_alloc(init_arr, sizes, arr_num);
    int item_quantity = input_alloc<int>(init_arr, sizes, arr_num);

    Heap<descriptor<int>*, pDescComparator<descriptor<int>*>> heap;
    init_heap_with_desc<int>(init_arr, sizes, arr_num, heap);

    int* result = new int[item_quantity];
    merge_arrays<int>(heap, result);
    output(result, item_quantity);

    mem_free(init_arr, sizes, arr_num);
    delete[] result;
    // del desc
    //  delete[] func
}

//дескрипторы в куче -- в каждом деск его массив, sizeб текущая позиция -- push desk -- top() -- pop() --
//++pos check pos == size -- push() -- while heap not e,pty