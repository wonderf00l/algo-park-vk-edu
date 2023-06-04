#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <set>
#include <unordered_map>
#include <utility>

const char field_size = 16;
const std::array<char, field_size> final_field = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class GameState {
   public:
    explicit GameState(const std::array<char, field_size>& field) : field(field), empty_pos(-1) {
        for (int i = 0; i != field_size; ++i) {
            if (field[i] == 0) {
                empty_pos = i;
                break;
            }
        }
        assert(empty_pos != -1);
    }
    bool is_final() const { return field == final_field; }
    bool is_solvable() const {
        int null_row = 0;

        for (int i = 0; i != 4; ++i) {
            for (int j = 0; j != 4; ++j) {
                if (field[4 * i + j] == 0) {
                    null_row = i;
                    break;
                }
            }
        }

        return !((inversion_count() + (null_row + 1)) % 2);
    }

    bool operator<(const GameState& other) const { return field < other.field; }
    bool operator==(const GameState& other) const { return field == other.field; }

    bool can_move_left() const { return empty_pos % 4 != 3; }

    bool can_move_right() const { return empty_pos % 4 != 0; }

    bool can_move_up() const { return empty_pos < 12; }

    bool can_move_down() const { return empty_pos > 3; }

    GameState move_left() const {
        assert(can_move_left());
        GameState new_state = GameState(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos + 1]);
        ++new_state.empty_pos;
        return new_state;
    }

    GameState move_right() const {
        assert(can_move_right());
        GameState new_state = GameState(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos - 1]);
        --new_state.empty_pos;
        return new_state;
    }

    GameState move_up() const {
        assert(can_move_up());
        GameState new_state = GameState(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos + 4]);
        new_state.empty_pos += 4;
        return new_state;
    }

    GameState move_down() const {
        assert(can_move_down());
        GameState new_state = GameState(*this);
        std::swap(new_state.field[empty_pos], new_state.field[empty_pos - 4]);
        new_state.empty_pos -= 4;
        return new_state;
    }

    size_t heuristic() const {
        size_t h = 0;

        for (int i = 0; i != field_size; ++i) {
            if (field[i] != i + 1) {
                ++h;
            }
        }

        return h;
    }

    friend std::string get_solution(const std::array<char, field_size>& field);
    friend struct GameStateHasher;
    friend std::ostream& operator<<(std::ostream& out, const GameState& state);

   private:
    std::array<char, field_size> field;
    char empty_pos;

    int inversion_count() const {
        int inversion_count = 0;

        for (int i = 0; i != field_size - 1; ++i) {
            for (int j = i + 1; j != field_size; ++j) {
                if (field[i] > field[j] && field[i] && field[j]) {
                    ++inversion_count;
                }
            }
        }

        return inversion_count;
    }
};

std::ostream& operator<<(std::ostream& out, const GameState& state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out << static_cast<int>(state.field[i * 4 + j]) << ' ';
        }
        out << std::endl;
    }
    return out;
}

struct GameStateHasher {
    size_t operator()(const GameState& state) const {
        size_t hash1 = 0, hash2 = 0;
        std::memcpy(&hash1, state.field.data(), sizeof(hash1));
        std::memcpy(&hash2, state.field.data() + field_size / 2, sizeof(hash2));
        return std::hash<size_t>{}(hash1 + hash2);
    }
};

std::string get_solution(const std::array<char, field_size>& field) {
    std::unordered_map<GameState, char, GameStateHasher> visited;
    std::set<std::pair<size_t, GameState>> priority_queue;

    GameState initial_state(field);

    if (!initial_state.is_solvable()) {
        return "-1";
    }

    visited[initial_state] = 'S';
    priority_queue.emplace(std::make_pair(0, initial_state));

    while (true) {
        std::pair<size_t, GameState> dist_n_state = *priority_queue.begin();
        priority_queue.erase(dist_n_state);

        if (dist_n_state.second.is_final()) {
            break;
        }

        if (dist_n_state.second.can_move_left()) {
            GameState new_state = dist_n_state.second.move_left();
            if (visited.find(new_state) == visited.end()) {
                visited[new_state] = 'L';
                priority_queue.emplace(std::make_pair(new_state.heuristic(), new_state));
            } else if (new_state.heuristic() < dist_n_state.first) {
                priority_queue.erase(std::make_pair(dist_n_state.first, dist_n_state.second));
                priority_queue.insert(std::make_pair(new_state.heuristic(), new_state));
            }
        }

        if (dist_n_state.second.can_move_right()) {
            GameState new_state = dist_n_state.second.move_right();
            if (visited.find(new_state) == visited.end()) {
                visited[new_state] = 'R';
                priority_queue.emplace(std::make_pair(new_state.heuristic(), new_state));
            } else if (new_state.heuristic() < dist_n_state.first) {
                priority_queue.erase(std::make_pair(dist_n_state.first, dist_n_state.second));
                priority_queue.insert(std::make_pair(new_state.heuristic(), new_state));
            }
        }

        if (dist_n_state.second.can_move_down()) {
            GameState new_state = dist_n_state.second.move_down();
            if (visited.find(new_state) == visited.end()) {
                visited[new_state] = 'D';
                priority_queue.emplace(std::make_pair(new_state.heuristic(), new_state));
            } else if (new_state.heuristic() < dist_n_state.first) {
                priority_queue.erase(std::make_pair(dist_n_state.first, dist_n_state.second));
                priority_queue.insert(std::make_pair(new_state.heuristic(), new_state));
            }
        }

        if (dist_n_state.second.can_move_up()) {
            GameState new_state = dist_n_state.second.move_up();
            if (visited.find(new_state) == visited.end()) {
                visited[new_state] = 'U';
                priority_queue.emplace(std::make_pair(new_state.heuristic(), new_state));
            } else if (new_state.heuristic() < dist_n_state.first) {
                priority_queue.erase(std::make_pair(dist_n_state.first, dist_n_state.second));
                priority_queue.insert(std::make_pair(new_state.heuristic(), new_state));
            }
        }
    }

    std::string path;
    GameState state(final_field);

    // std::cout << state << std::endl;
    while (visited[state] != 'S') {
        char move = visited[state];
        switch (move) {
            case 'L': {
                state = state.move_right();
                path += 'L';
                break;
            }
            case 'R': {
                state = state.move_left();
                path += 'R';
                break;
            }
            case 'D': {
                state = state.move_up();
                path += 'D';
                break;
            }
            case 'U': {
                state = state.move_down();
                path += 'U';
                break;
            }
        }

        // std::cout << state << std::endl;
    }

    std::reverse(path.begin(), path.end());
    std::cout << path.size() << std::endl;
    return path;
}

int main() {
    std::array<char, field_size> field;

    for (int i = 0; i != field_size; ++i) {
        int tmp = 0;
        std::cin >> tmp;
        field[i] = tmp;
    }

    std::cout << get_solution(field) << std::endl;
    return 0;
}