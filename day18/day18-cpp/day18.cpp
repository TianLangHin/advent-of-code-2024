#include<deque>
#include<fstream>
#include<iostream>
#include<map>
#include<sstream>
#include<string>
#include<utility>
#include<vector>

typedef std::pair<int, int> point_t;

int find_path(point_t start, point_t end, std::map<point_t, int>& obstacles, int limit) {
    std::map<point_t, int> best_costs;
    std::deque<std::pair<point_t, int>> frontier { { start, 0 } };

    while (!frontier.empty()) {
        auto state = frontier.front();
        frontier.pop_front();

        point_t point = state.first;
        int cost = state.second;

        if (point == end) {
            return cost;
        }

        if (best_costs.count(point)) {
            if (cost >= best_costs[point]) {
                continue;
            }
        }

        best_costs.insert({ point, cost });

        point_t neighbours[] = {
            { point.first - 1, point.second },
            { point.first + 1, point.second },
            { point.first, point.second - 1 },
            { point.first, point.second + 1 }
        };

        for (point_t next_pos : neighbours) {
            if (0 <= next_pos.first && next_pos.first <= end.first &&
                0 <= next_pos.second && next_pos.second <= end.second) {
                bool obstacle_hit = obstacles.count(next_pos) ?
                    obstacles[next_pos] < limit :
                    false;

                if (!obstacle_hit) {
                    frontier.push_back({ next_pos, cost + 1 });
                }
            }
        }
    }

    return -1;
}

void part1(std::map<point_t, int>& obstacles) {
    point_t start = { 0, 0 };
    point_t end = { 70, 70 };

    std::cout << find_path(start, end, obstacles, 1024) << "\n";
}

void part2(std::map<point_t, int>& obstacles, std::vector<point_t>& bytes) {
    point_t start = { 0, 0 };
    point_t end = { 70, 70 };

    int low = 0;
    int high = bytes.size();

    while (low < high) {
        int mid = (low + high) / 2;
        int result = find_path(start, end, obstacles, mid);
        if (result != -1) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    std::cout << bytes[low - 1].first << "," << bytes[low - 1].second << "\n";
}

int main() {
    std::fstream puzzle_input("day-18-puzzle-input.txt");
    std::string line;

    std::vector<point_t> bytes;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                std::stringstream ss;
                std::string section;
                ss << line << ',';

                std::getline(ss, section, ',');
                int x = std::stoi(section);
                std::getline(ss, section, ',');
                int y = std::stoi(section);

                bytes.push_back({ x, y });
            }
        }
    }

    std::map<point_t, int> obstacles;

    for (int i = 0; i < bytes.size(); ++i) {
        obstacles.insert({ bytes[i], i });
    }

    part1(obstacles);
    part2(obstacles, bytes);
}
