clang-format -i include/* *.cpp src/*
cppcheck --enable=all --suppress=missingIncludeSystem include/* *.cpp src/*
g++ -Wextra -Wall -std=c++17 -I./include -fsanitize=address -g *.cpp ./src/* -o executable
./executable