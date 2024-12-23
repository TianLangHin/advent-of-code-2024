#include<algorithm>
#include<deque>
#include<fstream>
#include<iostream>
#include<map>
#include<numeric>
#include<set>
#include<string>
#include<utility>
#include<vector>

typedef std::pair<int, int> point_t;

template <typename T, typename Predicate>
void retain(std::deque<T>& deque, Predicate pred) {
    auto it = deque.begin();
    while (it != deque.end()) {
        if (!pred(*it)) {
            it = deque.erase(it);
        } else {
            ++it;
        }
    }
}

template <typename T, typename Predicate>
void retain(std::vector<T>& vector, Predicate pred) {
    auto it = vector.begin();
    while (it != vector.end()) {
        if (!pred(*it)) {
            it = vector.erase(it);
        } else {
            ++it;
        }
    }
}

void part1(std::vector<std::string>& codes) {
    std::map<char, point_t> base_keypad {
        { '7', { 0, 0 } },
        { '8', { 1, 0 } },
        { '9', { 2, 0 } },
        { '4', { 0, 1 } },
        { '5', { 1, 1 } },
        { '6', { 2, 1 } },
        { '1', { 0, 2 } },
        { '2', { 1, 2 } },
        { '3', { 2, 2 } },
        { '0', { 1, 3 } },
        { 'A', { 2, 3 } },
    };
    std::map<char, point_t> dir_keypad {
        { '^', { 1, 0 } },
        { 'A', { 2, 0 } },
        { '<', { 0, 1 } },
        { 'v', { 1, 1 } },
        { '>', { 2, 1 } },
    };

    int answer = 0;

    for (std::string& code : codes) {

        std::deque<std::pair<std::string, int>> first_robot;
        first_robot.push_back({ "", 0 });

        for (int i = 0; i < code.size(); ++i) {
            char start_char = i == 0 ? 'A' : code[i - 1];
            char end_char = code[i];
            point_t start = base_keypad[start_char];
            point_t end = base_keypad[end_char];

            int x_start = start.first, y_start = start.second;
            int x_end = end.first, y_end = end.second;

            int x_dist = std::abs(x_end - x_start);
            int y_dist = std::abs(y_end - y_start);

            char x_key = x_start < x_end ? '>' : '<';
            char y_key = y_start < y_end ? 'v' : '^';

            std::string x_path = "", y_path = "";
            for (int j = 0; j < x_dist; ++j) {
                x_path += x_key;
            }
            for (int j = 0; j < y_dist; ++j) {
                y_path += y_key;
            }

            std::set<std::string> paths;
            for (auto& item : base_keypad) {
                auto p = item.second;
                if (p.first == x_end && p.second == y_start) {
                    paths.insert(x_path + y_path);
                    break;
                }
            }
            for (auto& item : base_keypad) {
                auto p = item.second;
                if (p.first == x_start && p.second == y_end) {
                    paths.insert(y_path + x_path);
                    break;
                }
            }

            while (!first_robot.empty()) {
                auto sol = first_robot.front();
                if (sol.second > i) {
                    break;
                }
                first_robot.pop_front();
                std::string item = sol.first;
                for (const auto& path : paths) {
                    first_robot.push_back(std::pair { item + path + "A", i + 1 });
                }
            }
        }

        int min_length = first_robot.front().first.size();
        for (const auto& robot : first_robot) {
            if (robot.first.size() < min_length) {
                min_length = robot.first.size();
            }
        }

        retain(
            first_robot,
            [&](const auto& item) {
                return item.first.size() == min_length;
            }
        );

        std::vector<std::pair<std::string, int>> robot;
        for (const auto& item : first_robot) {
            robot.push_back(item);
        }

        for (int j = 0; j < 2; ++j) {
            std::vector<std::pair<std::string, int>> next_robot;
            for (const auto& item : robot) {
                std::string code = item.first;
                std::deque<std::pair<std::string, int>> possible_solutions;
                possible_solutions.push_back({ "", 0 });

                for (int i = 0; i < code.size(); ++i) {
                    char start_char = i == 0 ? 'A' : code[i - 1];
                    char end_char = code[i];
                    point_t start = dir_keypad[start_char];
                    point_t end = dir_keypad[end_char];

                    int x_start = start.first, y_start = start.second;
                    int x_end = end.first, y_end = end.second;

                    int x_dist = std::abs(x_end - x_start);
                    int y_dist = std::abs(y_end - y_start);

                    char x_key = x_start < x_end ? '>' : '<';
                    char y_key = y_start < y_end ? 'v' : '^';

                    std::string x_path = "", y_path = "";
                    for (int j = 0; j < x_dist; ++j) {
                        x_path += x_key;
                    }
                    for (int j = 0; j < y_dist; ++j) {
                        y_path += y_key;
                    }

                    std::set<std::string> paths;
                    for (const auto& item : dir_keypad) {
                        auto p = item.second;
                        if (p.first == x_end && p.second == y_start) {
                            paths.insert(x_path + y_path);
                            break;
                        }
                    }
                    for (const auto& item : dir_keypad) {
                        auto p = item.second;
                        if (p.first == x_start && p.second == y_end) {
                            paths.insert(y_path + x_path);
                            break;
                        }
                    }

                    while (!possible_solutions.empty()) {
                        auto sol = possible_solutions.front();
                        if (sol.second > i) {
                            break;
                        }
                        possible_solutions.pop_front();
                        std::string item = sol.first;
                        for (const auto& path : paths) {
                            possible_solutions.push_back(std::pair { item + path + "A", i + 1 });
                        }
                    }
                }

                for (const auto& item : possible_solutions) {
                    next_robot.push_back(item);
                }
            }

            int min_length = next_robot.front().first.size();
            for (const auto& robot : next_robot) {
                if (robot.first.size() < min_length) {
                    min_length = robot.first.size();
                }
            }

            retain(
                next_robot,
                [&](const auto& item) {
                    return item.first.size() == min_length;
                }
            );

            robot.clear();
            for (const auto& item : next_robot) {
                robot.push_back(item);
            }
        }

        int length = robot.front().first.size();
        for (const auto& s : robot) {
            if (s.first.size() < length) {
                length = s.first.size();
            }
        }

        int numeric_part = (code[0] - '0') * 100 + (code[1] - '0') * 10 + (code[2] - '0');

        answer += length * numeric_part;
    }

    std::cout << answer << "\n";
}

int main() {
    std::fstream puzzle_input("day-21-puzzle-input.txt");
    std::string line;
    std::vector<std::string> codes;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                codes.push_back(line);
            }
        }
    }

    part1(codes);
}
