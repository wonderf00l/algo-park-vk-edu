#include <cmath>
#include <iostream>

bool is_simple(int num) {
    if (num == 1) {
        return false;
    }
    for (int i = 2; i * i <= num; ++i) {
        if (!(num % i)) {
            return false;
        }
    }
    return true;
}

int main() {
    for (int i = std::pow(2, 30);; ++i) {
        if (is_simple(i)) {
            std::cout << i << std::endl;
            break;
        }
    }
}