#include<fstream>
#include<iostream>
#include<numeric>
#include<string>
#include<utility>
#include<vector>

void part1(const std::vector<std::pair<std::vector<int>, bool>> schematics) {
    int answer = 0;
    for (const auto& lock_item : schematics) {
        std::vector<int> lock = lock_item.first;
        bool is_lock = lock_item.second;
        for (const auto& key_item : schematics) {
            std::vector<int> key = key_item.first;
            bool is_not_key = key_item.second;
            if (is_lock && !is_not_key) {
                bool fitting = true;
                for (int i = 0; i < lock.size(); ++i) {
                    fitting = fitting && lock[i] + key[i] <= 5;
                }
                if (fitting) {
                    ++answer;
                }
            }
        }
    }
    std::cout << answer << "\n";
}

int main() {
    std::fstream puzzle_input("day-25-puzzle-input.txt");
    std::string line;

    std::vector<std::string> rows;
    std::vector<std::pair<std::vector<int>, bool>> schematics;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (line.empty()) {
                int length = rows[0].size();
                bool is_lock = true;
                for (const char& s : rows[0]) {
                    is_lock = is_lock && s == '#';
                }
                std::vector<int> heights;
                for (int n = 0; n < length; ++n) {
                    int count = 0;
                    for (int i = 0; i < rows.size(); ++i) {
                        if (rows[i][n] == '#') {
                            ++count;
                        }
                    }
                    heights.push_back(count - 1);
                }
                schematics.push_back(std::pair { heights, is_lock });
                rows.clear();
            } else {
                rows.push_back(line);
            }
        }
    }

    part1(schematics);
}
