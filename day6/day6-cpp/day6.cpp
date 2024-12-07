#include<algorithm>
#include<fstream>
#include<iostream>
#include<map>
#include<optional>
#include<set>
#include<utility>
#include<vector>

typedef std::pair<int, int> point;
typedef std::map<int, std::set<int>> obstacle_map;
typedef std::pair<obstacle_map, obstacle_map> obstacle_maps;

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

Direction next_direction(Direction d) {
    switch (d) {
        case UP:
            return RIGHT;
        case RIGHT:
            return DOWN;
        case DOWN:
            return LEFT;
        case LEFT:
            return UP;
    }
    __builtin_unreachable();
}

obstacle_maps make_maps(std::vector<point> obstacles) {
    obstacle_map row_map, col_map;
    for (auto p : obstacles) {
        int row = p.first;
        int col = p.second;
        if (row_map.count(row)) {
            row_map[row].insert(col);
        } else {
            row_map[row] = std::set<int> { col };
        }
        if (col_map.count(col)) {
            col_map[col].insert(row);
        } else {
            col_map[col] = std::set<int> { row };
        }
    }
    return obstacle_maps(row_map, col_map);
}

template<bool TRACKING>
void make_move(int obstacle, Direction direction, std::set<point>& visited, point *pos) {
    switch (direction) {
        case UP:
            if (TRACKING) {
                for (int i = obstacle + 1; i < pos->first; ++i) {
                    visited.insert(point(i, pos->second));
                }
            }
            *pos = point(obstacle + 1, pos->second);
            break;
        case RIGHT:
            if (TRACKING) {
                for (int i = pos->second; i < obstacle; ++i) {
                    visited.insert(point(pos->first, i));
                }
            }
            *pos = point(pos->first, obstacle - 1);
            break;
        case DOWN:
            if (TRACKING) {
                for (int i = pos->first; i < obstacle; ++i) {
                    visited.insert(point(i, pos->second));
                }
            }
            *pos = point(obstacle - 1, pos->second);
            break;
        case LEFT:
            if (TRACKING) {
                for (int i = obstacle + 1; i < pos->second; ++i) {
                    visited.insert(point(pos->first, i));
                }
            }
            *pos = point(pos->first, obstacle + 1);
            break;
    }
}

std::optional<int> get_next_obstacle(
    Direction direction,
    point pos,
    obstacle_map row_map,
    obstacle_map col_map) {

    switch (direction) {
        case UP:
            if (col_map.count(pos.second)) {
                auto obstacle = std::upper_bound(
                    col_map[pos.second].rbegin(),
                    col_map[pos.second].rend(),
                    pos.first,
                    [](int target, const int& coordinate) {
                        return target > coordinate;
                    }
                );
                return obstacle != col_map[pos.second].rend() ?
                    std::optional<int> { *obstacle } :
                    std::nullopt;
            } else {
                return std::nullopt;
            }
        case RIGHT:
            if (row_map.count(pos.first)) {
                auto obstacle = std::upper_bound(
                    row_map[pos.first].begin(),
                    row_map[pos.first].end(),
                    pos.second,
                    [](int target, const int&coordinate) {
                        return target < coordinate;
                    }
                );
                return obstacle != row_map[pos.first].end() ?
                    std::optional<int> { *obstacle } :
                    std::nullopt;
            } else {
                return std::nullopt;
            }
        case DOWN:
            if (col_map.count(pos.second)) {
                auto obstacle = std::upper_bound(
                    col_map[pos.second].begin(),
                    col_map[pos.second].end(),
                    pos.first,
                    [](int target, const int&coordinate) {
                        return target < coordinate;
                    }
                );
                return obstacle != col_map[pos.second].end() ?
                    std::optional<int> { *obstacle } :
                    std::nullopt;
            } else {
                return std::nullopt;
            }
        case LEFT:
            if (row_map.count(pos.first)) {
                auto obstacle = std::upper_bound(
                    row_map[pos.first].rbegin(),
                    row_map[pos.first].rend(),
                    pos.second,
                    [](int target, const int& coordinate) {
                        return target > coordinate;
                    }
                );
                return obstacle != row_map[pos.first].rend() ?
                    std::optional<int> { *obstacle } :
                    std::nullopt;
            } else {
                return std::nullopt;
            }
    }
    __builtin_unreachable();
}

std::set<point> part1(std::vector<point> obstacles, point start, int rows, int cols) {
    auto maps = make_maps(obstacles);
    auto row_map = maps.first;
    auto col_map = maps.second;

    point pos = start;
    std::set<point> visited { start };
    Direction direction = UP;

    std::optional<int> next_obstacle = get_next_obstacle(direction, pos, row_map, col_map);

    while (next_obstacle.has_value()) {
        make_move<true>(next_obstacle.value(), direction, visited, &pos);
        direction = next_direction(direction);
        next_obstacle = get_next_obstacle(direction, pos, row_map, col_map);
    }

    switch (direction) {
        case UP:
            for (int i = 0; i < pos.first; ++i) {
                visited.insert(point(i, pos.second));
            }
        case RIGHT:
            for (int i = pos.second; i < cols; ++i) {
                visited.insert(point(pos.first, i));
            }
        case DOWN:
            for (int i = pos.first; i < rows; ++i) {
                visited.insert(point(i, pos.second));
            }
        case LEFT:
            for (int i = 0; i < pos.second; ++i) {
                visited.insert(point(pos.first, i));
            }
    }

    std::cout << visited.size() << "\n";

    visited.erase(start);

    return visited;
}

void part2(
    std::vector<point> obstacles,
    point start,
    std::set<point> possible_placements) {

    int sum = 0;

    std::set<point> visited;

    auto maps = make_maps(obstacles);
    auto row_map = maps.first;
    auto col_map = maps.second;

    for (auto p : possible_placements) {
        int x = p.first;
        int y = p.second;

        if (row_map.count(x)) {
            row_map[x].insert(y);
        } else {
            row_map[x] = std::set<int> { y };
        }

        if (col_map.count(y)) {
            col_map[y].insert(x);
        } else {
            col_map[y] = std::set<int> { x };
        }

        bool overlapped = false;

        point pos = start;
        Direction direction = UP;
        std::set<std::pair<point, Direction>> orientations { { start, UP } };

        std::optional<int> next_obstacle = get_next_obstacle(direction, pos, row_map, col_map);

        while (next_obstacle.has_value()) {
            make_move<false>(next_obstacle.value(), direction, visited, &pos);
            direction = next_direction(direction);
            if (orientations.count({ pos, direction })) {
                overlapped = true;
                break;
            }
            orientations.insert({ pos, direction });
            next_obstacle = get_next_obstacle(direction, pos, row_map, col_map);
        }

        if (overlapped) {
            sum++;
        }

        row_map[x].erase(y);
        col_map[y].erase(x);
    }

    std::cout << sum << "\n";
}

int main() {
    int cols = 0;
    int rows = 0;
    point start = std::pair<int, int>(0, 0);

    std::fstream puzzle_input("day-6-puzzle-input.txt");
    std::string line;
    std::vector<point> obstacles;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                for (int i = 0; i < line.length(); ++i) {
                    if (line[i] == '^') {
                        start = point(rows, i);
                    } else if (line[i] == '#') {
                        obstacles.push_back(point(rows, i));
                    }
                }
            }
            ++rows;
        }
    }

    auto path = part1(obstacles, start, rows, cols);
    part2(obstacles, start, path);
}
