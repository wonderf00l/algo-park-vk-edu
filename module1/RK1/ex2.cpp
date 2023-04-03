#include <iostream>
#include <vector>
#include <algorithm>

void output_(const std::vector<size_t>& arr) {
    for (const int a : arr) {
        std::cout << a << std::endl;
    }
}

struct box {
    int length;
    int width;
    int height;
    int id;

    bool operator<(const box& other) const {
        std::vector<int> this_arr = {this->length, this->width, this->height}; // WILL WORK!
        std::vector<int> other_arr = {other.length, other.width, other.height};

        std::vector<size_t> id_arr;
        id_arr.reserve(this_arr.size());
        for (const int& attr : this_arr) {
            if (auto greater_id = std::upper_bound(other_arr.begin(), other_arr.end(), attr); 
            greater_id != other_arr.end() && std::find(id_arr.begin(), id_arr.end(), greater_id - other_arr.begin()) == id_arr.end()) {
                id_arr.push_back(greater_id - other_arr.begin());
            }
            // если тот же индекс, делаем отступ до след элемента, если не дошли до конца, др индекс найден, иначе - нельзя подобрать(false)
        }
        output_(id_arr);
        if (id_arr.size() == this_arr.size()) {
            return true;
        }
        else {
            return false;
        }
        // return this->length < other.length && this->width < other.width && this->height < other.height;

    }
};

void input(std::vector<box>& box_arr) {
    for (size_t i = 0; i != box_arr.size(); ++i) {
        std::cin >> box_arr[i].length >> box_arr[i].width >> box_arr[i].height;
        box_arr[i].id = i;
    }
}

void sort_boxes(std::vector<box>& box_arr) {
    for (size_t i = 1; i != box_arr.size(); ++i) {
        box tmp = box_arr[i];
        int j = i - 1;
        for (; j >= 0 && tmp < box_arr[j]; --j) {
            box_arr[j + 1] = box_arr[j];
        }
        box_arr[j + 1] = tmp;
    }
}

template<typename T = box>
void output(const std::vector<T>& box_arr) {
    for (size_t i = 0; i != box_arr.size(); ++i) {
        i != box_arr.size() - 1 ? std::cout << box_arr[i].id << ' ' : std::cout << box_arr[i].id << std::endl;
    }
}

int main() {
    int quantity = 0;
    std::cin >> quantity;

    std::vector<box> box_arr(quantity);

    input(box_arr);

    sort_boxes(box_arr);

    output(box_arr);
}