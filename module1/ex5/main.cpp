#include <cassert>
#include <iostream>

template <typename T>
struct DefaultComparator {
    bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
};

struct segment {
    int begin, end;
    bool operator<(const segment& other) const { return begin < other.begin; }
    // bool operator<(const segment& other) {
    //     if (begin < other.begin) {
    //         return true;
    //     }
    //     else if (begin == other.begin && end < other.end) {

    //     }
    //     else {
    //         return false;
    //     }
    // }
};

void input(segment* seg_arr, int* points, int size) {
    for (int i = 0, j = 0; i != size; ++i, j += 2) {
        std::cin >> seg_arr[i].begin >> seg_arr[i].end;
        assert(seg_arr[i].begin < seg_arr[i].end);
        points[j] = seg_arr[i].begin;
        points[j + 1] = seg_arr[i].end;
    }
}

template <typename T, typename Comparator = DefaultComparator<T>>
void merge(T* arr, T* buf, int left, int right, Comparator comp = Comparator()) {
    if (left < right) {
        int middle = (left + right) / 2;
        merge(arr, buf, left, middle);
        merge(arr, buf, middle + 1, right);

        int buf_i = left;
        for (int i = left, j = middle + 1; i <= middle || j <= right;) {
            if (j > right || (i <= middle && comp(arr[i], arr[j]))) {
                buf[buf_i++] = arr[i++];
            } else {
                buf[buf_i++] = arr[j++];
            }
        }

        for (int i = left; i <= right; ++i) {
            arr[i] = buf[i];
        }
    }
}

template <typename T, typename Comparator = DefaultComparator<T>>
void merge_sort(T* arr, int size, Comparator = Comparator()) {
    if (arr && size) {
        T* buf = new T[size];
        merge(arr, buf, 0, size - 1);
        delete[] buf;
    }
}

bool in_segment(const segment* seg, int point) { return point > seg->begin && point <= seg->end; }

// int min(const segment* seg_arr, int size) {
//     int min_p = seg_arr[0].begin;
//     for (int i = 0; i != size; ++i) {
//         if (seg_arr[i].begin < m)
//     }
// }//add comp maybe points arr

int count_len(segment* seg_arr, int* points, int size) {
    int len = 0;
    bool on_seg = 0;

    for (int point = points[0]; point <= points[size * 2 - 1]; ++point) {
        on_seg = 0;
        for (int i = 0; i != size; ++i) {
            if (in_segment(seg_arr + i, point)) {
                on_seg = 1;
                break;
            }
        }
        if (on_seg) {
            // std::cout << "point: " << point << std::endl;
            ++len;
        }
    }

    return len;
}

int main() {
    int N = 0;
    std::cin >> N;
    assert(N > 0);

    segment* seg_arr = new segment[N];
    int* points = new int[N * 2];

    input(seg_arr, points, N);

    merge_sort<segment>(seg_arr, N);
    merge_sort<int>(points, N * 2);
    // std::cout << "seg: " << seg_arr[2].begin << std::endl;

    std::cout << count_len(seg_arr, points, N) << std::endl;

    delete[] seg_arr;
    delete[] points;
}