#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
const int a = 173099;

int Hash_Function(string s, int table_size) {
    int hash = s[0] % table_size;
    for (auto item = s.begin() + 1; item != s.end(); ++item) hash = (hash * a + *item) % table_size;
    return hash;
}

class HashTable {
   public:
    HashTable(int size_) : table(size_), size(0) {
        for (int i = 0; i < table.size(); ++i) table[i] = " ";
    };
    bool Insert(string s) {
        if (4 * size >= table.size() * 3) grow();
        int hash = Hash_Function(s, table.size());
        for (int i = 0; i < table.size(); ++i) {
            hash = (hash + i) % table.size();
            // std::cout << s << '\t' <<hash << '\n';
            if (table[hash] == " ") {
                table[hash] = s;
                break;
            }
            if (table[hash] == s) return false;
        }
        ++size;
        return true;
    }
    bool erase(string s) {
        int n = table.size();
        int hash = Hash_Function(s, table.size());
        for (int i = 0;; ++i) {
            hash = (hash + i) % table.size();
            if (table[hash] == s) {
                table[hash] = "DELETED";
                return true;
            }
            if (table[hash] == " ") return false;
        }
        return false;
    }
    bool find(string s) {
        int n = table.size();
        int hash = Hash_Function(s, table.size());
        for (int i = 0;; ++i) {
            hash = (hash + i) % table.size();
            if (table[hash] == s) return true;
            if (table[hash] == " ") return false;
        }
    }

   private:
    vector<std::string> table;
    int size;
    void grow() {
        // std::cout << "grow" << '\n';
        vector<string> New_Table(table.size() * 2);
        for (int i = 0; i < New_Table.size(); ++i)  // cannot i use this . Can i use Hash_Table?
            New_Table[i] = " ";
        int size1 = 0;
        for (int i = 0; i < table.size(); ++i)
            if (table[i] != "DELETED" && table[i] != " ") {
                int hash = Hash_Function(table[i], New_Table.size());
                for (int j = 0;; ++j) {
                    hash = (hash + j) % New_Table.size();
                    if (New_Table[hash] == " ") {
                        ++size1;
                        New_Table[hash] = table[i];
                        break;
                    }
                }
            }
        table = New_Table;
        size = size1;
    }
};
int main() {
    HashTable tb(8);
    string c;
    while (std::cin >> c) {
        string s;
        std::cin >> s;
        if (c == "+") {
            if (tb.Insert(s) == 1)
                std::cout << "OK" << '\n';
            else
                std::cout << "FAIL" << '\n';
            continue;
        }
        if (c == "?") {
            if (tb.find(s) == 1)
                std::cout << "OK" << '\n';
            else
                std::cout << "FAIL" << '\n';
            continue;
        }
        if (tb.erase(s) == 1)
            std::cout << "OK" << '\n';
        else
            std::cout << "FAIL" << '\n';
    }
}