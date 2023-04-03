#include <math.h>

#include <iostream>
#include <vector>

//если семантика объектов/типов одинакова -- вставка в шаблон проходит --
//происходит генерация кода то есть разные типы со схожей семантикой(callable,
//кол-во аргументов, типы аргументов и тд) приемлемы для шаблонов
template <typename T>
struct DefaultComparator {
    bool operator()(const T &l, const T &r) const { return l < r; }
};

template <typename T, typename Comparator = DefaultComparator<T>>
class Heap {
   public:
    Heap(Comparator comp = Comparator())
        : comp(comp)  //,......
    {}
    Heap(const T &arr, size_t arr_size, Comparator comp = Comparator())
        : comp(comp)  //,......
    {}
    ~Heap();

    const T &top();
    void pop();
    void push(const T &);

    bool is_empty() const { return heap_size == 0; }
    size_t size() const { return heap_size; }

   private:
    void heapify();
    void sift_up();
    void sift_down();

    Comparator comp;

    T *buf;
    size_t buf_size;
    size_t heap_size;
};

template <typename T, typename Comparator = DefaultComparator<T>>
void sort(T *arr, size_t size,
          Comparator &comp) {  // если передавать указатель на функцию, то
                               // программа не скомпилируется + нельзя
                               // формировать default компаратор по умолчанию
    size_t l = 0;
    size_t r = size;

    for (size_t i = l; i < r; ++i) {
        for (size_t j = l; j < r - 1; ++j) {
            if (comp(arr[j + 1], arr[j])) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

template <typename T>
void sort_simple(T *arr, size_t size, bool (*comp)(const T &l, const T &r)) {
    size_t l = 0;
    size_t r = size;

    for (size_t i = l; i < r; ++i) {
        for (size_t j = l; j < r - 1; ++j) {
            if (comp(arr[j + 1], arr[j])) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

struct Point {
    int x;
    int y;

    bool operator<(const Point &r) const {  // define comparasion
        return this->x < r.x;
    }
};

template <typename T>  // 1 нормальный способ: определить шаблонный компаратор и
                       // передавать его в аргументы, для сложный типов
                       // определяем operator<
bool is_less(const T &l, const T &r) {
    return l < r;
}

bool is_less_points(const Point &l, const Point &r) { return l.x < r.x; }

bool is_less_points_by_y(const Point &l, const Point &r) { return l.y < r.y; }

struct ComparatorPoint {
    Point base_point;
    // int* compare_count; // keyword: mutable
    int compare_count;

    // ComparatorPoint(const Point& bp, int* compare_count) : base_point(bp),
    // compare_count(compare_count) {

    // }

    ComparatorPoint(const Point &bp) : base_point(bp), compare_count(0) {}

    bool operator()(const Point &l, const Point &r) {
        //(*compare_count)++;
        ++compare_count;
        return abs(base_point.x - l.x) < abs(base_point.x - r.x);
    }
};

int main() {
    std::vector<int> arr = {5, 4, 3, 2, 7, 11, 100, -100, 80, 0};
    // int*
    sort(&arr[0], arr.size(), is_less<int>);  // sort<int> or auto type detection
    // can call function like that insted of (*comp)(args) so it works
    for (int c : arr) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    int counter = 0;
    // ComparatorPoint comp_points(Point({0,0}), &counter);
    ComparatorPoint comp_points(Point({0, 0}));

    std::vector<Point> arr_points = {{0, 0}, {1, 1}, {10, 0}, {0, 20}};
    // sort(&arr_points[0], arr_points.size()); // with default object of the
    // dedault template(type) of the comparator
    // ComparatorPoint::operator<(ComparatorPoint* this, const Point& lhs, const
    // Point& rhs) -- won't work(different func signature) but static methods may
    // work

    sort(&arr_points[0], arr_points.size(), comp_points);

    // don't forget lambda
    for (Point c : arr_points) {
        std::cout << "{" << c.x << "," << c.y << "} ";
    }
    std::cout << std::endl;

    // std::cout << *comp_points.compare_count << std::endl;
    std::cout << comp_points.compare_count << std::endl;
}