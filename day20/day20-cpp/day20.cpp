#include<cmath>
#include<deque>
#include<fstream>
#include<iostream>
#include<map>
#include<set>
#include<string>
#include<utility>
#include<vector>

typedef std::pair<int, int> point_t;

template <typename T>
bool contains(const std::vector<T>& vec, const T& item) {
    for (const auto& element : vec) {
        if (element == item) {
            return true;
        }
    }
    return false;
}

std::vector<point_t> find_best_path(point_t start, point_t end, const std::set<point_t>& free_spaces) {
    std::deque<std::pair<point_t, std::vector<point_t>>> frontier { { start, std::vector { start } } };

    while (!frontier.empty()) {
        auto state = frontier.front();
        frontier.pop_front();

        point_t point = state.first;
        std::vector<point_t> path = state.second;

        if (point == end) {
            return path;
        }

        point_t neighbours[] = {
            { point.first - 1, point.second },
            { point.first + 1, point.second },
            { point.first, point.second - 1 },
            { point.first, point.second + 1 }
        };

        for (const point_t& next_point : neighbours) {
            if (free_spaces.count(next_point) && !contains(path, next_point)) {
                path.push_back(next_point);
                frontier.push_back({ next_point, path });
                path.pop_back();
            }
        }
    }

    __builtin_unreachable();
}

void part1(point_t start, point_t end, const std::set<point_t>& free_spaces) {
    std::map<point_t, int> steps;
    auto best_path = find_best_path(start, end, free_spaces);
    for (int i = 0; i < best_path.size(); ++i) {
        steps.insert({ best_path[i], i });
    }

    int cheats = 0;

    for (const auto& pair : steps) {
        point_t p = pair.first;

        point_t neighbours[] = {
            { p.first, p.second - 2 },
            { p.first - 1, p.second - 1 },
            { p.first + 1, p.second - 1 },
            { p.first - 2, p.second },
            { p.first + 2, p.second },
            { p.first - 1, p.second + 1 },
            { p.first + 1, p.second + 1 },
            { p.first, p.second + 2 }
        };

        for (const auto& n : neighbours) {
            if (n > p) {
                if (steps.count(p) && steps.count(n)) {
                    int step1 = steps[p];
                    int step2 = steps[n];
                    if (std::abs(step2 - step1) >= 102) {
                        ++cheats;
                    }
                }
            }
        }
    }

    std::cout << cheats << "\n";
}

int manhattan_distance(point_t x, point_t y) {
    return std::abs(x.first - y.first) + std::abs(x.second - y.second);
}

void part2(point_t start, point_t end, const std::set<point_t>& free_spaces) {
    std::map<point_t, int> steps;
    auto best_path = find_best_path(start, end, free_spaces);
    for (int i = 0; i < best_path.size(); ++i) {
        steps.insert({ best_path[i], i });
    }

    int cheats = 0;

    for (const auto& first_item : steps) {
        for (const auto& second_item : steps) {
            point_t cheat_start = first_item.first;
            point_t cheat_end = second_item.first;

            int step_start = first_item.second;
            int step_end = second_item.second;

            int distance_cheat = manhattan_distance(cheat_start, cheat_end);

            if (step_start < step_end && distance_cheat <= 20) {
                int distance_diverted = step_end - step_start;
                if (distance_diverted - distance_cheat >= 100) {
                    ++cheats;
                }
            }
        }
    }

    std::cout << cheats << "\n";

}

int main() {
    point_t start = { 0, 0 };
    point_t end = { 0, 0 };
    std::set<point_t> free_spaces;

    std::fstream puzzle_input("day-20-puzzle-input.txt");
    std::string line;

    int y = 0;
    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                int x = 0;
                for (char& c : line) {
                    if (c == 'S') {
                        free_spaces.insert({ x, y });
                        start = { x, y };
                    } else if (c == 'E') {
                        free_spaces.insert({ x, y });
                        end = { x, y };
                    } else if (c == '.') {
                        free_spaces.insert({ x, y });
                    }
                    ++x;
                }
            }
            ++y;
        }
    }

    part1(start, end, free_spaces);
    part2(start, end, free_spaces);
}
