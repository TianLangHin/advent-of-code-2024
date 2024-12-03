#include<fstream>
#include<iostream>
#include<regex>
#include<string>

void part1(std::string line) {
    std::regex pattern("mul\\((\\d+)\\,(\\d+)\\)");
    int answer = 0;
    auto matches_begin = std::sregex_iterator(line.begin(), line.end(), pattern);
    auto matches_end = std::sregex_iterator();
    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
        std::smatch match = *i;
        answer += std::stoi(match[1]) * std::stoi(match[2]);
    }
    std::cout << answer << "\n";
}

void part2(std::string line) {
    std::regex pattern("mul\\(\\d+\\,\\d+\\)|do\\(\\)|don't\\(\\)");
    std::regex mul("mul\\((\\d+)\\,(\\d+)\\)");

    bool enabled = true;
    int sum = 0;
    auto matches_begin = std::sregex_iterator(line.begin(), line.end(), pattern);
    auto matches_end = std::sregex_iterator();
    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
        std::string match_string = (*i).str();
        if (match_string == "do()") {
            enabled = true;
            continue;
        } else if (match_string == "don't()") {
            enabled = false;
            continue;
        }
        if (enabled) {
            std::smatch nums;
            std::regex_match(match_string, nums, mul);
            sum += std::stoi(nums[1]) * std::stoi(nums[2]);
        }
    }
    std::cout << sum << "\n";
}

int main() {
    std::ifstream puzzle_input("day-3-puzzle-input.txt");
    std::string line, fused_input = "";

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                fused_input += line;
            }
        }
    }

    part1(fused_input);
    part2(fused_input);
}
