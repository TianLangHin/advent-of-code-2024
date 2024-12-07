#include<fstream>
#include<iostream>
#include<sstream>
#include<stack>
#include<string>
#include<utility>
#include<vector>

typedef unsigned long long long_int;

void part1(std::vector<std::pair<long_int, std::vector<long_int>>> equations) {
    long_int sum = 0;
    for (auto pair : equations) {
        auto target = pair.first;
        auto nums = pair.second;
        std::stack<std::pair<long_int, long_int>> frontier;
        frontier.push({ nums[0], 1 });
        while (!frontier.empty()) {
            auto top_element = frontier.top();
            frontier.pop();
            auto result = top_element.first;
            auto idx = top_element.second;
            if (result == target && idx == nums.size()) {
                sum += result;
                break;
            }
            if (idx < nums.size()) {
                frontier.push({ result + nums[idx], idx + 1 });
                frontier.push({ result * nums[idx], idx + 1 });
            }
        }
    }
    std::cout << sum << "\n";
}

long_int concat(long_int a, long_int b) {
    int digits = 0, c = b;
    while (b) {
        b /= 10;
        digits++;
    }
    while (digits--) {
        a *= 10;
    }
    return a + c;
}

void part2(std::vector<std::pair<long_int, std::vector<long_int>>> equations) {
    long_int sum = 0;
    for (auto pair : equations) {
        auto target = pair.first;
        auto nums = pair.second;
        std::stack<std::pair<long_int, long_int>> frontier;
        frontier.push({ nums[0], 1 });
        while (!frontier.empty()) {
            auto top_element = frontier.top();
            frontier.pop();
            auto result = top_element.first;
            auto idx = top_element.second;
            if (result == target && idx == nums.size()) {
                sum += result;
                break;
            }
            if (idx < nums.size()) {
                frontier.push({ result + nums[idx], idx + 1 });
                frontier.push({ result * nums[idx], idx + 1 });
                frontier.push({ concat(result, nums[idx]), idx + 1 });
            }
        }
    }
    std::cout << sum << "\n";
}

int main() {
    std::fstream puzzle_input("day-7-puzzle-input.txt");
    std::string line;

    std::vector<std::pair<long_int, std::vector<long_int>>> equations;

    if (puzzle_input.is_open()) {
        while (puzzle_input.good()) {
            std::getline(puzzle_input, line);
            if (!line.empty()) {
                std::stringstream ss;
                ss << line << ' ';

                std::string section;
                std::getline(ss, section, ':');
                long_int test = std::stoll(section);

                std::vector<long_int> nums;

                std::getline(ss, section, ' ');
                std::getline(ss, section, ' ');

                while (!section.empty()) {
                    nums.push_back(std::stoll(section));
                    std::getline(ss, section, ' ');
                }

                equations.push_back({ test, nums });
            }
        }
    }
    part1(equations);
    part2(equations);
}
