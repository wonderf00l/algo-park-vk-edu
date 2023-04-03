//Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. Для каждого элемента
//массива B[i] найдите минимальный индекс k минимального элемента массива A, равного или превосходящего B[i]:
//A[k] >= B[i]. Если такого элемента нет, выведите n. Время работы поиска k для каждого элемента B[i]:
//O(log(k)). n, m ≤ 10000.

#include <cassert>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

void input_size(size_t &a_size, std::istream &input_stream) { input_stream >> a_size; }

void fill_arr(std::vector<unsigned int> &arr, std::istream &input_stream) {
    for (unsigned int &item : arr) {
        input_stream >> item;
    }
}

template <typename T = unsigned int>
void output(const std::vector<T> &arr, std::ostream &output_stream) {
    for (size_t i = 0; i != arr.size(); ++i) {
        i != arr.size() - 1 ? output_stream << arr[i] << ' ' : output_stream << arr[i] << std::endl;
    }
}

std::pair<size_t, size_t> exp_search(const std::vector<unsigned int> &arr, unsigned int item) {
    assert(arr.size());

    size_t left = 0, right = 1;
    if (item < arr[left]) {
        return std::make_pair(left, left);
    } else if (item > arr[arr.size() - 1]) {
        return std::make_pair(arr.size(), arr.size());
    }

    while ((right < arr.size()) && arr[right] < item) {
        left = right;
        right *= 2;
    }

    if (right > arr.size() - 1) {
        right = arr.size() - 1;
    }

    return std::make_pair(left, right + 1);
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
    // assert(B.size());

    std::vector<size_t> id_arr;
    id_arr.reserve(B.size());

    for (const unsigned int &arr_item : B) {
        id_arr.push_back(find_min_id(A, arr_item));
    }

    return id_arr;
}

int run_search_min(std::istream &input_stream, std::ostream &output_stream) {
    size_t a_size = 0, b_size = 0;
    input_size(a_size, input_stream);
    input_size(b_size, input_stream);

    std::vector<unsigned int> A(a_size);
    std::vector<unsigned int> B(b_size);

    fill_arr(A, input_stream);
    fill_arr(B, input_stream);

    std::vector<size_t> id_arr = find_indexes(A, B);

    output<size_t>(id_arr, output_stream);

    return 0;
}

void test_logic() {
    {
        std::stringstream input;
        std::stringstream output;

        input << "2 1\n"
              << "1 2\n"
              << "2\n";
        run_search_min(input, output);
        std::cout << output.str();
        assert(output.str() == "1\n");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "4 3\n"
              << "2 4 5 7\n"
              << "4 6 1\n";
        run_search_min(input, output);
        std::cout << output.str();
        assert(output.str() == "1 3 0\n");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "1 1\n"
              << "2\n"
              << "2\n";
        run_search_min(input, output);
        std::cout << output.str();
        assert(output.str() == "0\n");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "1 0\n"
              << "2\n";
        run_search_min(input, output);
        std::cout << output.str();
        assert(output.str() == "");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "2 2\n"
              << "0 0\n"
              << "0 1 \n";
        run_search_min(input, output);
        std::cout << output.str();
        assert(output.str() == "0 2\n");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "4 9\n"
              << "1 4 7 9\n"
              << "0 1 2 4 5 7 8 9 10\n";
        run_search_min(input, output);
        std::cout << output.str();
        assert(output.str() == "0 0 1 1 2 2 3 3 4\n");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "3 4\n"
              << "1 1 1\n"
              << "0 1 1 2\n";
        run_search_min(input, output);
        std::cout << output.str();
        assert(output.str() == "0 0 0 3\n");
    }
}

int main() {
    // test_logic();
    return run_search_min(std::cin, std::cout);
}