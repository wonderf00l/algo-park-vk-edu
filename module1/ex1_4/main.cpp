// Инвертируйте значение бита в числе N по его номеру K.

// Необходимо использование битовых операций.
// Использование арифметических операций запрещено.

#include <cassert>
#include <iostream>

short input(long long int& num, short& bit_num) {
    std::cin >> num >> bit_num;
    assert(bit_num >= 0);
    return bit_num;
}

void invert(long long int& num, short& bit_num) { num ^= (1UL << input(num, bit_num)); }

int main() {
    long long int num = 0;
    short bit_num = 0;

    // int a = 55;
    // if ((a >> 4) & 1U) {
    //     std::cout << "bit with 1";
    // }
    invert(num, bit_num);

    std::cout << num << std::endl;
}