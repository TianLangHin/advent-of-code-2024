#include<cmath>
#include<fstream>
#include<iostream>
#include<set>
#include<string>
#include<unordered_map>
#include<utility>
#include<vector>

typedef std::pair<int, int> point;
typedef std::unordered_map<char, std::vector<point>> antenna_list;

void part1(const antenna_list& antennae, int rows, int cols) {
    std::set<point> antinodes;
    for (const auto& entry : antennae) {
        auto frequency = entry.first;
        auto locations = entry.second;
        for (int i = 0; i < locations.size(); ++i) {
            for (int j = i+1; j < locations.size(); ++j) {
                int x_distance = locations[j].first - locations[i].first;
                int y_distance = std::abs(locations[j].second - locations[i].second);
                if (x_distance % 3 == 0 && y_distance % 3 == 0) {
                    point point_near_i = {
                        locations[i].first + x_distance / 3,
                        locations[i].second < locations[j].second ?
                            locations[i].second + y_distance / 3 :
                            locations[i].second - y_distance / 3
                    };
                    point point_near_j = {
                        locations[j].first - x_distance / 3,
                        locations[j].second < locations[i].second ?
                            locations[j].second + y_distance / 3 :
                            locations[j].second - y_distance / 3
                    };
                    antinodes.insert(point_near_i);
                    antinodes.insert(point_near_j);
                }
                point beyond_i = {
                    locations[i].first - (locations[j].first - locations[i].first),
                    locations[i].second - (locations[j].second - locations[i].second)
                };
                point beyond_j = {
                    locations[j].first - (locations[i].first - locations[j].first),
                    locations[j].second - (locations[i].second - locations[j].second)
                };
                if (0 <= beyond_i.first && beyond_i.first < rows &&
                    0 <= beyond_i.second && beyond_i.second < cols)
                    antinodes.insert(beyond_i);
                if (0 <= beyond_j.first && beyond_j.first < rows &&
                    0 <= beyond_j.second && beyond_j.second < cols)
                    antinodes.insert(beyond_j);
            }
        }
    }
    std::cout << antinodes.size() << "\n";
}

int gcd(int a, int b) {
    int t;
    while (b) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

void part2(const antenna_list& antennae, int rows, int cols) {
    std::set<point> antinodes;
    for (const auto& entry : antennae) {
        auto frequency = entry.first;
        auto locations = entry.second;
        for (int i = 0; i < locations.size(); ++i) {
            for (int j = i+1; j < locations.size(); ++j) {
                int x_distance = locations[j].first - locations[i].first;
                int y_distance = std::abs(locations[j].second - locations[i].second);
                int y_direction = locations[i].second < locations[j].second ? 1 : -1;
                int divisor = gcd(x_distance, y_distance);

                point p = locations[i];
                while (0 <= p.first && p.first < rows &&
                    0 <= p.second && p.second < cols) {
                    antinodes.insert(p);
                    p = {
                        p.first + x_distance / divisor,
                        p.second + y_direction * y_distance / divisor
                    };
                }
                p = locations[i];
                while (0 <= p.first && p.first < rows &&
                    0 <= p.second && p.second < cols) {
                    antinodes.insert(p);
                    p = {
                        p.first - x_distance / divisor,
                        p.second - y_direction * y_distance / divisor
                    };
                }
            }
        }
    }
    std::cout << antinodes.size() << "\n";
}

int main() {
    antenna_list antennae;
    int rows = 0, cols = 0;

    std::fstream puzzle_input("day-8-puzzle-input.txt");
    std::string line;

    if (puzzle_input.is_open()) {
        while(puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                int i = 0;
                for (char& c : line) {
                    if (c != '.') {
                        point p(rows, i);
                        if (antennae.count(c)) {
                            antennae[c].push_back(p);
                        } else {
                            antennae[c] = { p };
                        }
                    }
                    ++i;
                }
                cols = line.size();
                rows++;
            }
        }
    }
    part1(antennae, rows, cols);
    part2(antennae, rows, cols);
}
