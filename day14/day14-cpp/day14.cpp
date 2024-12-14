#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<utility>
#include<vector>

typedef std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> robots_specs;

void part1(robots_specs& robots) {
    const int width = 101;
    const int height = 103;

    int quadrant[] = {0, 0, 0, 0};

    for (auto& spec : robots) {
        int px = spec.first.first;
        int py = spec.first.second;
        int vx = spec.second.first;
        int vy = spec.second.second;

        int result_x = ((px % width) + 100 * (vx % width)) % width;
        if (result_x < 0)
            result_x += width;

        int result_y = ((py % height) + 100 * (vy % height)) % height;
        if (result_y < 0)
            result_y += height;

        int index = 0;

        if (result_x > width / 2)
            index += 2;
        if (result_y > height / 2)
            index++;

        if (result_x != width / 2 && result_y != height / 2)
            quadrant[index]++;
    }

    int answer = quadrant[0] * quadrant[1] * quadrant[2] * quadrant[3];

    std::cout << answer << "\n";
}

int main() {
    std::fstream puzzle_input("day-14-puzzle-input.txt");
    std::string line;

    robots_specs robots;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            std::stringstream ss;
            ss << line;
            if (!line.empty()) {
                int px, py, vx, vy;
                std::getline(ss, line, '=');
                std::getline(ss, line, ',');
                px = std::stoi(line);
                std::getline(ss, line, ' ');
                py = std::stoi(line);
                std::getline(ss, line, '=');
                std::getline(ss, line, ',');
                vx = std::stoi(line);
                std::getline(ss, line);
                vy = std::stoi(line);

                robots.push_back({{ px, py }, { vx, vy }});
            }
        }
    }
    part1(robots);
}
