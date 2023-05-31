clang-format -i *.cpp
cppcheck --enable=all --suppress=missingIncludeSystem *.cpp
g++ -Wextra -Wall -std=c++17 -fsanitize=address -g for_contest.cpp -o executable