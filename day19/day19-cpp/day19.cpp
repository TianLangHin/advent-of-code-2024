#include<algorithm>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>

void part1(std::vector<std::string>& towels, std::vector<std::string>& designs) {
    int answer = 0;
    for (std::string& design : designs) {
        std::vector<bool> solvable(design.size());
        std::fill(solvable.begin(), solvable.end(), false);
        for (int i = 0; i < design.size(); ++i) {
            for (std::string& towel : towels) {
                if (i + 1 >= towel.size()) {
                    int start_index = i + 1 - towel.size();
                    bool matches_suffix = true;
                    for (int j = start_index; j < i + 1; ++j) {
                        matches_suffix = matches_suffix && design[j] == towel[j - start_index];
                    }
                    bool previous_solutions = start_index == 0 || solvable[start_index - 1];
                    if (matches_suffix && previous_solutions) {
                        solvable[i] = true;
                        break;
                    }
                }
            }
        }
        if (solvable.back())
            ++answer;
    }
    std::cout << answer << "\n";
}

void part2(std::vector<std::string>& towels, std::vector<std::string>& designs) {
    long long int answer = 0;
    for (std::string& design : designs) {
        std::vector<long long int> solvable(design.size());
        std::fill(solvable.begin(), solvable.end(), 0);
        for (int i = 0; i < design.size(); ++i) {
            for (std::string& towel : towels) {
                if (i + 1 >= towel.size()) {
                    int start_index = i + 1 - towel.size();
                    bool matches_suffix = true;
                    for (int j = start_index; j < i + 1; ++j) {
                        matches_suffix = matches_suffix && design[j] == towel[j - start_index];
                    }
                    if (matches_suffix) {
                        solvable[i] += start_index == 0 ? 1 : solvable[start_index - 1];
                    }
                }
            }
        }
        answer += solvable.back();
    }
    std::cout << answer << "\n";
}

int main() {
    std::fstream puzzle_input("day-19-puzzle-input.txt");
    std::string line;

    std::vector<std::string> towels;
    std::vector<std::string> designs;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (towels.empty()) {
                std::stringstream ss;
                ss << line << ", ";

                std::string towel;
                std::getline(ss, towel, ',');
                while (!towel.empty()) {
                    towels.push_back(towel);
                    std::getline(ss, towel, ' ');
                    std::getline(ss, towel, ',');
                }
            } else if (!line.empty()) {
                designs.push_back(line);
            }
        }
    }
    part1(towels, designs);
    part2(towels, designs);
}
