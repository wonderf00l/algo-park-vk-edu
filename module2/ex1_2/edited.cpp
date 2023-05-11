#include <cassert>
#include <iostream>
#include <string>
#include <vector>

/*Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые
строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с помощью вычисления
значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при
добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна поддерживать
операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
Вариант 2. Для разрешения коллизий используйте двойное хеширование.*/

template <typename Key>
struct DefaultComparator {
    bool operator()(const Key& lhs, const Key& rhs) const { return lhs == rhs; }
};

template <class Key>
class Hasher {};

template <>
class Hasher<std::string> {
   public:
    size_t operator()(const std::string& value, size_t m) const { return str_hash(value, m); }

   private:
    size_t str_hash(const std::string& str, size_t m) const {
        size_t hash = 0;
        int a = m - 1;  // a и m: взамно-простые числа
        for (char ch : str) {
            hash = (hash * a + ch) % m;
        }
        return hash;
    }
    // size_t str_hash(const std::string& str, size_t m) const {
    //     int hash = 0;
    //     for (int i = str.size() - 1; i >= 0; i--)
    //     {
    //         hash += (11 * hash + str[i]) % m;
    //     }
    //     return hash % m;
    // }
};

template <class Key>
class ProbHasher {};

template <>
class ProbHasher<std::string> {
   public:
    size_t operator()(int hash) const { return (hash % 2) ? hash : hash + 1; }

   private:
    // size_t str_hash(const std::string& str, size_t m) const {
    //     size_t hash = 0;
    //     int a = m - 1;
    //     for (char ch : str) {
    //         hash = (hash * a + ch) % m;
    //     }
    //     if (!(hash % 2)) {  // h2(k) должна возвращать нечетные значения
    //         hash += 1;
    //     }
    //     return hash;
    // }
    // size_t str_hash(const std::string& str, size_t m) const {
    //     // return !(hash % 2) ? hash + 1 : hash;
    //     int hash = 0;
    //     for (int i = str.size() - 1; i >= 0; i--)
    //     {
    //         hash += (13 * hash + str[i]) % m;
    //     }
    //     return (hash * 2 + 1) % m;
    // }
};

template <typename Key, typename Hasher = Hasher<Key>, typename ProbHasher = ProbHasher<Key>,
          typename Comparator = DefaultComparator<Key>>
class HashTable {
    enum class states { EMPTY, DELETED, FILLED };

    struct Bucket {
        Key key;
        states status;
        Bucket() : key(Key()), status(states::EMPTY) {}
        Bucket(const Key& key, const states& stat) : key(key), status(stat) {}
        friend std::ostream& operator<<(std::ostream& stream, const Bucket& bucket) {
            stream << bucket.key;
            return stream;
        }
    };

   public:
    HashTable() : buckets_size(8), buckets_quantity(0) { buckets.resize(buckets_size); }
    HashTable(const HashTable& other)
        : buckets(other.buckets), buckets_size(other.buckets_size), buckets_quantity(other.buckets_quantity) {}
    HashTable& operator=(const HashTable& other) {
        buckets = other.buckets;
        buckets_size = other.buckets_size;
        buckets_quantity = other.buckets_quantity;
        return *this;
    }

    HashTable(HashTable&& other)
        : buckets(std::move(other.buckets)), buckets_size(other.buckets_size), buckets_quantity(other.buckets_quantity) {}
    HashTable& operator=(HashTable&& other) {
        buckets = std::move(other.buckets);
        buckets_size = other.buckets_size;
        buckets_quantity = other.buckets_quantity;
        return *this;
    }

    ~HashTable() = default;

    size_t size() const { return buckets_quantity; }

    bool operator[](const Key& key) const {
        if (!buckets.size()) {
            return false;
        }
        size_t hash1 = hasher(key, buckets_size), hash2 = prob_hasher(hash1);
        for (size_t i = 1; buckets[hash1].status != states::EMPTY && i <= buckets_size;
             hash1 = prob_step(hash1, hash2, i++)) {
            if (comp(buckets[hash1].key, key)) {
                return true;
            }
        }
        return false;
    }

    bool add(const Key& key) {
        // if (operator[](key)) {
        //     return false;
        // }

        if (4 * buckets_quantity >=
            buckets_size * 3) {  // static_cast<float>(buckets_quantity) >= load_factor * static_cast<float>(buckets_size)
            rehash();
        }

        size_t hash1 = hasher(key, buckets_size), hash2 = prob_hasher(hash1), first_deleted = 0;
        bool got_deleted = 0;

        for (size_t i = 1; buckets[hash1].status != states::EMPTY; hash1 = prob_step(hash1, hash2, i++)) {
            if (comp(buckets[hash1].key, key)) {  // buckets[hash1].status == states::FILLED -- предусмотрено
                return false;
            }

            if (buckets[hash1].status == states::DELETED && !got_deleted) {
                first_deleted = hash1;
                got_deleted = 1;
            }
        }

        if (got_deleted) {
            buckets[first_deleted].key = key;
            buckets[first_deleted].status = states::FILLED;
            ++buckets_quantity;
        } else {
            buckets[hash1].key = key;
            buckets[hash1].status = states::FILLED;
            ++buckets_quantity;
            // std::cout << buckets[id].key;
        }
        return true;
    }

    bool erase(const Key& key) {
        // if (!operator[](key)) {
        //     return false;
        // }

        size_t hash1 = hasher(key, buckets_size), hash2 = prob_hasher(hash1);

        for (size_t i = 1; buckets[hash1].status != states::EMPTY && i <= buckets_size;
             hash1 = prob_step(hash1, hash2, i++)) {
            if (comp(buckets[hash1].key, key)) {
                buckets[hash1].status = states::DELETED;
                buckets[hash1].key = Key();
                --buckets_quantity;
                return true;
            }
        }

        return false;
    }

    friend std::ostream& operator<<(std::ostream& stream, const HashTable& table) {
        for (size_t i = 0; i != table.buckets_size; ++i) {
            if (table.buckets[i].status == states::FILLED) {
                (i != table.buckets_size - 1) ? stream << table.buckets[i] << " " : stream << table.buckets[i] << std::endl;
            }
        }
        return stream;
    }

   private:
    void rehash() {
        buckets_size *= 2;
        std::vector<Bucket> new_buckets(buckets_size);
        for (Bucket& bucket : buckets) {
            if (bucket.status == states::FILLED) {
                size_t hash1 = hasher(bucket.key, buckets_size), hash2 = prob_hasher(hash1);

                for (size_t i = 1; new_buckets[hash1].status == states::FILLED; hash1 = prob_step(hash1, hash2, i++)) {
                }

                new_buckets[hash1] = std::move(bucket);
            }
        }
        buckets = std::move(new_buckets);
    }

    size_t prob_step(int hash1, int hash2, size_t i) const { return (hash1 + i * hash2) % buckets_size; }

    Hasher hasher;
    ProbHasher prob_hasher;
    Comparator comp;
    std::vector<Bucket> buckets;
    size_t buckets_size;
    size_t buckets_quantity;
};

int main() {
    HashTable<std::string> table;
    // assert(!table["doesn't exist"]);
    // assert(table.add("hello"));
    // assert(table.add("world"));
    // assert(table["hello"]);
    // assert(table["world"]);
    // assert(table.erase("hello"));
    // assert(!table.erase("hello"));
    // assert(!table["hello"]);
    // assert(!table.add("world"));
    // assert(table.erase("world"));
    // assert(!table.erase("world"));

    // for (int i = 0; i != 5000; ++i) {
    //     assert(table.add(std::to_string(i)));
    // }

    // for (int i = 0; i != 5000; ++i) {
    //     assert(table[std::to_string(i)]);
    // }

    // for (int i = 0; i != 5000; ++i) {
    //     std::cout << "num: " << i << std::endl;
    //     assert(table.erase(std::to_string(i)));
    // }

    // for (int i = 0; i != 5000; ++i) {
    //     assert(!table[std::to_string(i)]);
    // }

    // std::cout << table;

    std::string key;
    char operation;
    while (std::cin >> operation >> key) {
        bool result = false;
        switch (operation) {
            case '+':
                result = table.add(key);
                break;
            case '-':
                result = table.erase(key);
                break;
            case '?':
                result = (table[key]);
                break;
            default:
                break;
        }
        if (result) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }

    return 0;
}