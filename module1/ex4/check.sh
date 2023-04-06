clang-format -i src/main.cpp comp/*/* heap/*/*
cppcheck --enable=all --suppress=missingIncludeSystem src/* comp/src/* comp/include/* heap/src/* heap/include/*