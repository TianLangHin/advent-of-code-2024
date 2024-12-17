#include<algorithm>
#include<fstream>
#include<functional>
#include<iostream>
#include<map>
#include<set>
#include<string>
#include<tuple>
#include<utility>
#include<vector>

typedef std::pair<int, int> point_t;

enum Direction {
    LEFT, RIGHT, UP, DOWN
};

void part1(point_t start, point_t end, const std::set<point_t>& free_spaces) {
    std::map<std::pair<point_t, Direction>, int> lowest_score {
        { { start, RIGHT }, 0 },
    };
    std::vector<std::tuple<int, point_t, Direction>> frontier {
        { 0, start, RIGHT },
    };

    while (!frontier.empty()) {
        std::make_heap(frontier.begin(), frontier.end(), std::greater<>());
        std::pop_heap(frontier.begin(), frontier.end(), std::greater<>());
        auto item = frontier.back();
        frontier.pop_back();

        int score = std::get<0>(item);
        point_t point = std::get<1>(item);
        Direction direction = std::get<2>(item);

        if (point == end) {
            std::cout << score << "\n";
            break;
        }

        std::tuple<point_t, Direction, Direction> neighbours[] = {
            { { point.first - 1, point.second }, LEFT, RIGHT },
            { { point.first + 1, point.second }, RIGHT, LEFT },
            { { point.first, point.second - 1 }, UP, DOWN },
            { { point.first, point.second + 1 }, DOWN, UP },
        };

        for (auto& neighbour : neighbours) {
            point_t cell = std::get<0>(neighbour);
            Direction move_direction = std::get<1>(neighbour);
            Direction inverse_direction = std::get<2>(neighbour);

            if (free_spaces.count(cell) != 0 && direction != inverse_direction) {
                int added_score = direction == move_direction ? 1 : 1001;
                int new_score = score + added_score;
                std::pair<point_t, Direction> key = { cell, move_direction };
                if (lowest_score.count(key) != 0) {
                    int recorded_score = lowest_score[key];
                    if (recorded_score > new_score) {
                        lowest_score.insert({ key, new_score });
                        frontier.push_back({ new_score, cell, move_direction });
                        std::make_heap(frontier.begin(), frontier.end(), std::greater<>());
                    }
                } else {
                    lowest_score.insert({ key, new_score });
                    frontier.push_back({ new_score, cell, move_direction });
                    std::make_heap(frontier.begin(), frontier.end(), std::greater<>());
                }
            }
        }
    }
}

int main() {
    point_t start = { 0, 0 };
    point_t end = { 0, 0 };
    std::set<point_t> free_spaces;

    std::fstream puzzle_input("day-16-puzzle-input.txt");
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
}
