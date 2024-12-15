#include<deque>
#include<fstream>
#include<iostream>
#include<set>
#include<string>
#include<utility>
#include<vector>

typedef std::pair<int, int> point_t;
typedef std::set<point_t> points_t;

enum Direction {
    LEFT, RIGHT, UP, DOWN
};

void part1(point_t robot, points_t walls, points_t boxes, std::vector<Direction> steps) {
    for (Direction step : steps) {
        int x = robot.first;
        int y = robot.second;

        switch (step) {
            case LEFT:
                --x;
                break;
            case RIGHT:
                ++x;
                break;
            case UP:
                --y;
                break;
            case DOWN:
                ++y;
                break;
        }

        std::vector<point_t> found_boxes;
        bool wall_hit = walls.count({ x, y }) != 0;
        bool box_hit = boxes.count({ x, y }) != 0;

        while (wall_hit || box_hit) {
            if (wall_hit) {
                break;
            } else if (box_hit) {
                found_boxes.push_back({ x, y });
            }
            switch (step) {
                case LEFT:
                    --x;
                    break;
                case RIGHT:
                    ++x;
                    break;
                case UP:
                    --y;
                    break;
                case DOWN:
                    ++y;
                    break;
            }
            wall_hit = walls.count({ x, y }) != 0;
            box_hit = boxes.count({ x, y }) != 0;
        }
        if (!wall_hit) {
            for (point_t& b : found_boxes)
                boxes.erase(b);
            switch (step) {
                case LEFT:
                    for (auto& b : found_boxes)
                        boxes.insert({ b.first - 1, b.second });
                    robot = { robot.first - 1, robot.second };
                    break;
                case RIGHT:
                    for (auto& b : found_boxes)
                        boxes.insert({ b.first + 1, b.second });
                    robot = { robot.first + 1, robot.second };
                    break;
                case UP:
                    for (auto& b : found_boxes)
                        boxes.insert({ b.first, b.second - 1 });
                    robot = { robot.first, robot.second - 1 };
                    break;
                case DOWN:
                    for (auto& b : found_boxes)
                        boxes.insert({ b.first, b.second + 1 });
                    robot = { robot.first, robot.second + 1 };
                    break;
            }
        }
    }

    int answer = 0;
    for (point_t box : boxes) {
        answer += 100 * box.second + box.first;
    }

    std::cout << answer << "\n";
}

void part2(
    point_t initial_robot,
    points_t& initial_walls,
    points_t& initial_boxes,
    std::vector<Direction> steps) {

    point_t robot = { initial_robot.first * 2, initial_robot.second };
    points_t walls, boxes;

    for (const point_t& point : initial_walls) {
        walls.insert({ point.first * 2, point.second });
        walls.insert({ point.first * 2 + 1, point.second });
    }

    for (const point_t& point : initial_boxes) {
        boxes.insert({ point.first * 2, point.second });
    }

    for (Direction& step : steps) {
        if (step == LEFT || step == RIGHT) {
            int inc = step == LEFT ? -1 : 0;
            int skip = step == LEFT ? -1 : 1;
            int x = robot.first;
            int y = robot.second;
            x += skip;
            std::vector<point_t> found_boxes;
            bool wall_hit = walls.count({ x, y }) != 0;
            bool box_hit = boxes.count({ x + inc, y }) != 0;
            while (wall_hit || box_hit) {
                if (wall_hit) {
                    break;
                } else if (box_hit) {
                    found_boxes.push_back({ x + inc, y });
                    x += 2 * skip;
                }
                wall_hit = walls.count({ x, y }) != 0;
                box_hit = boxes.count({ x + inc, y }) != 0;
            }
            if (!wall_hit) {
                for (point_t& b : found_boxes) {
                    boxes.erase(b);
                }
                for (point_t& b : found_boxes) {
                    boxes.insert({ b.first + skip, b.second });
                }
                robot = { robot.first + skip, robot.second };
            }
        } else if (step == UP || step == DOWN) {
            int inc = step == UP ? -1 : 1;
            std::vector<point_t> visited;
            std::deque<point_t> frontier;
            if (walls.count({ robot.first, robot.second + inc })) {
                continue;
            }
            if (boxes.count({ robot.first, robot.second + inc })) {
                visited.push_back({ robot.first, robot.second + inc });
                frontier.push_back({ robot.first, robot.second + inc });
            }
            if (boxes.count({ robot.first - 1, robot.second + inc })) {
                visited.push_back({ robot.first - 1, robot.second + inc });
                frontier.push_back({ robot.first - 1, robot.second + inc });
            }
            bool wall_hit = false;
            while (!frontier.empty()) {
                point_t b = frontier.front();
                frontier.pop_front();
                if (walls.count({ b.first, b.second + inc }) != 0
                    || walls.count({ b.first + 1, b.second + inc }) != 0) {

                    wall_hit = true;
                    break;
                }
                for (int x_value : { b.first - 1, b.first, b.first + 1 }) {
                    if (boxes.count({ x_value, b.second + inc })) {
                        visited.push_back({ x_value, b.second + inc });
                        frontier.push_back({ x_value, b.second + inc });
                    }
                }
            }
            if (!wall_hit) {
                for (point_t& point : visited) {
                    boxes.erase(point);
                }
                for (point_t& point : visited) {
                    boxes.insert({ point.first, point.second + inc });
                }
                robot = { robot.first, robot.second + inc };
            }
        }
    }

    int answer = 0;
    for (point_t box : boxes) {
        answer += 100 * box.second + box.first;
    }

    std::cout << answer << "\n";
}

int main() {
    std::fstream puzzle_input("day-15-puzzle-input.txt");
    std::string line;

    points_t walls, boxes;
    point_t robot = { 0, 0 };
    std::vector<Direction> steps;

    int i = 0;
    bool section = true;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (section) {
                if (line.empty()) {
                    section = false;
                } else {
                    int j = 0;
                    for (char& c : line) {
                        switch (c) {
                            case '#':
                                walls.insert({ j, i });
                                break;
                            case 'O':
                                boxes.insert({ j, i });
                                break;
                            case '@':
                                robot = { j, i };
                                break;
                        }
                        ++j;
                    }
                    ++i;
                }
            } else {
                for (char& c : line) {
                    switch (c) {
                        case '^':
                            steps.push_back(UP);
                            break;
                        case '>':
                            steps.push_back(RIGHT);
                            break;
                        case 'v':
                            steps.push_back(DOWN);
                            break;
                        case '<':
                            steps.push_back(LEFT);
                            break;
                    }
                }
            }
        }
    }

    part1(robot, walls, boxes, steps);
    part2(robot, walls, boxes, steps);
}
