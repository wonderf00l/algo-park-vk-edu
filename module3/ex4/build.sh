clang-format -i *.cpp cppcheck-- enable = all-- suppress = missingIncludeSystem main.cpp g++ - Wextra - Wall - std =c++ 17 - fsanitize =
                                                                      address - g main.cpp - o executable./ executable