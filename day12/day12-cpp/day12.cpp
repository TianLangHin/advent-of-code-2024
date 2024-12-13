#include<algorithm>
#include<deque>
#include<fstream>
#include<iostream>
#include<map>
#include<set>
#include<string>
#include<utility>
#include<vector>

typedef std::pair<int, int> point_t;
typedef point_t (*point_fn)(point_t);

enum Direction {
    UP, DOWN, LEFT, RIGHT
};

point_t up(point_t p) { return { p.first - 1, p.second }; }
point_t down(point_t p) { return { p.first + 1, p.second }; }
point_t left(point_t p) { return { p.first, p.second - 1 }; }
point_t right(point_t p) { return { p.first, p.second + 1 }; }

const std::pair<point_fn, Direction> NEIGHBOURS[] = {
    { up, UP },
    { down, DOWN },
    { left, LEFT },
    { right, RIGHT }
};

const std::pair<point_fn, Direction> ADJACENTS[] = {
    { right, UP },
    { right, DOWN },
    { down, LEFT },
    { down, RIGHT },
};

template <typename K, typename V, typename Predicate>
void retain(std::map<K, V>& dict, Predicate pred) {
    auto it = dict.begin();
    while (it != dict.end()) {
        if (!pred(it->first)) {
            it = dict.erase(it);
        } else {
            ++it;
        }
    }
}

template <typename T, typename Predicate>
void retain(std::set<T>& container, Predicate pred) {
    auto it = container.begin();
    while (it != container.end()) {
        if (!pred(*it)) {
            it = container.erase(it);
        } else {
            ++it;
        }
    }
}

void part1(std::vector<std::vector<char>>& farm) {
    std::map<char, std::deque<std::pair<int, int>>> plant_coord_map;
    int rows = farm.size();
    int cols = farm[0].size();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (plant_coord_map.count(farm[i][j])) {
                plant_coord_map[farm[i][j]].push_back({ i, j });
            } else {
                plant_coord_map[farm[i][j]] = { { i, j } };
            }
        }
    }

    int total_price = 0;

    for (auto& entry : plant_coord_map) {
        char plant_type = entry.first;
        auto& coords = entry.second;

        while (!coords.empty()) {
            auto point = coords.front();
            coords.pop_front();

            int area = 1;
            std::map<point_t, int> outer_squares;
            for (int i = 0; i < 4; ++i) {
                outer_squares[(NEIGHBOURS[i].first)(point)] = 1;
            }

            std::set<point_t> visited { point };
            std::deque<point_t> frontier { point };

            while (!frontier.empty()) {
                point_t p = frontier.front();
                frontier.pop_front();

                point_t next_points[] = {
                    { std::max(0, p.first - 1), p.second },
                    { std::min(rows - 1, p.first + 1), p.second },
                    { p.first, std::max(0, p.second - 1) },
                    { p.first, std::min(cols - 1, p.second + 1) }
                };
                for (auto& next_point : next_points) {
                    if (visited.count(next_point) == 0
                        && farm[next_point.first][next_point.second] == plant_type) {

                        for (int i = 0; i < 4; ++i) {
                            auto transformed_point = (NEIGHBOURS[i].first)(next_point);
                            if (outer_squares.count(transformed_point)) {
                                ++outer_squares[transformed_point];
                            } else {
                                outer_squares[transformed_point] = 1;
                            }
                        }

                        coords.erase(std::find(coords.begin(), coords.end(), next_point));

                        ++area;
                        visited.insert(next_point);
                        frontier.push_back(next_point);
                    }
                }
            }
            retain(outer_squares, [&](auto point) { return visited.count(point) == 0; });

            int perimeter = 0;
            for (auto& square : outer_squares) {
                perimeter += square.second;
            }
            total_price += area * perimeter;
        }
    }

    std::cout << total_price << "\n";
}

void part2(std::vector<std::vector<char>>& farm) {
    std::map<char, std::deque<std::pair<int, int>>> plant_coord_map;
    int rows = farm.size();
    int cols = farm[0].size();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (plant_coord_map.count(farm[i][j])) {
                plant_coord_map[farm[i][j]].push_back({ i, j });
            } else {
                plant_coord_map[farm[i][j]] = { { i, j } };
            }
        }
    }

    int total_price = 0;

    for (auto& entry : plant_coord_map) {
        char plant_type = entry.first;
        auto& coords = entry.second;

        while (!coords.empty()) {
            auto point = coords.front();
            coords.pop_front();

            int area = 1;
            std::set<std::pair<point_t, Direction>> outer_squares;
            for (int i = 0; i < 4; ++i) {
                outer_squares.insert({
                    (NEIGHBOURS[i].first)(point),
                    NEIGHBOURS[i].second
                });
            }

            std::set<point_t> visited { point };
            std::deque<point_t> frontier { point };

            while (!frontier.empty()) {
                point_t p = frontier.front();
                frontier.pop_front();

                point_t next_points[] = {
                    { std::max(0, p.first - 1), p.second },
                    { std::min(rows - 1, p.first + 1), p.second },
                    { p.first, std::max(0, p.second - 1) },
                    { p.first, std::min(cols - 1, p.second + 1) }
                };
                for (auto& next_point : next_points) {
                    if (visited.count(next_point) == 0
                        && farm[next_point.first][next_point.second] == plant_type) {

                        for (int i = 0; i < 4; ++i) {
                            outer_squares.insert({
                                (NEIGHBOURS[i].first)(next_point),
                                NEIGHBOURS[i].second
                            });
                        }

                        coords.erase(std::find(coords.begin(), coords.end(), next_point));

                        ++area;
                        visited.insert(next_point);
                        frontier.push_back(next_point);
                    }
                }
            }
            retain(outer_squares, [&](auto p) { return visited.count(p.first) == 0; });

            int number_of_sides = 0;
            while (!outer_squares.empty()) {
                auto squares = *outer_squares.begin();
                point_t sample_point = squares.first;
                Direction direction = squares.second;

                point_fn func = ADJACENTS[0].first;
                for (int i = 0; i < 4; ++i) {
                    if (ADJACENTS[i].second == direction) {
                        func = ADJACENTS[i].first;
                    }
                }

                outer_squares.erase(std::find(
                    outer_squares.begin(),
                    outer_squares.end(),
                    std::pair { sample_point, direction }
                ));

                point_t p = sample_point;
                while (outer_squares.count(std::pair { func(p), direction }) > 0) {
                    p = func(p);
                    outer_squares.erase(std::find(
                        outer_squares.begin(),
                        outer_squares.end(),
                        std::pair { p, direction }
                    ));
                }

                ++number_of_sides;
            }

            total_price += area * number_of_sides;
        }
    }

    std::cout << total_price << "\n";
}

int main() {
    std::fstream puzzle_input("day-12-puzzle-input.txt");
    std::string line;
    std::vector<std::vector<char>> farm;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                std::vector<char> row;
                for (char& c : line)
                    row.push_back(c);
                farm.push_back(row);
            }
        }
    }

    part1(farm);
    part2(farm);
}
